#include "mysqlpool.h"


MysqlPool* MysqlPool::mysqlpool_object = NULL;
std::mutex MysqlPool::objectlock;
std::mutex MysqlPool::poollock;

MysqlPool::MysqlPool() {}

/*
 *配置数据库参数
 */
void MysqlPool::setParameter( const char*   mysqlhost,
                              const char*   mysqluser,
                              const char*   mysqlpwd,
                              const char*   databasename,
                              unsigned int  port,
                              const char*   socket,
                              unsigned long client_flag,
                              unsigned int  max_connect ) {
  _mysqlhost    = mysqlhost;
  _mysqluser    = mysqluser;
  _mysqlpwd     = mysqlpwd;
  _databasename = databasename;
  _port         = port;
  _socket       = socket;
  _client_flag  = client_flag;
  MAX_CONNECT   = max_connect;
}
  
/*
 *有参的单例函数，用于第一次获取连接池对象，初始化数据库信息。
 */
MysqlPool* MysqlPool::getMysqlPoolObject() {
  if (mysqlpool_object == NULL) { 
    objectlock.lock();
    if (mysqlpool_object == NULL) {
      mysqlpool_object = new MysqlPool();
    }
    objectlock.unlock();
  }
  return mysqlpool_object;
}
                                                 
/*
 *创建一个连接对象
 */
MYSQL* MysqlPool::createOneConnect() {
  MYSQL* conn = NULL;
  conn = mysql_init(conn);
  if (conn != NULL) {
    if (mysql_real_connect(conn,
                          _mysqlhost,
                          _mysqluser,
                          _mysqlpwd,
                          _databasename,
                          _port,
                          _socket,
                          _client_flag)) {
      connect_count++;
      return conn;   
    } else {
      std::cout << mysql_error(conn) << std::endl;
      return NULL;
    }
  } else {
    std::cerr << "init failed" << std::endl;
    return NULL;
  }
}

/*
 *判断当前MySQL连接池的是否空
 */
bool MysqlPool::isEmpty() {
  return mysqlpool.empty();
}
/*
 *获取当前连接池队列的队头
 */
MYSQL* MysqlPool::poolFront() {
  return mysqlpool.front();
}
/*
 *
 */
unsigned int MysqlPool::poolSize() {
  return mysqlpool.size();
}
/*
 *弹出当前连接池队列的队头
 */
void MysqlPool::poolPop() {
  mysqlpool.pop();
}
/*
 *获取连接对象，如果连接池中有连接，就取用;没有，就重新创建一个连接对象。
 *同时注意到MySQL的连接的时效性，即在连接队列中,连接对象在超过一定的时间后没有进行操作，
 *MySQL会自动关闭连接，当然还有其他原因，比如：网络不稳定，带来的连接中断。
 *所以在获取连接对象前，需要先判断连接池中连接对象是否有效。
 *考虑到数据库同时建立的连接数量有限制，在创建新连接需提前判断当前开启的连接数不超过设定值。
 */
MYSQL* MysqlPool::getOneConnect() {
  poollock.lock();
  MYSQL *conn = NULL;
  if (!isEmpty()) {
    while (!isEmpty() && mysql_ping(poolFront())) {
      mysql_close(poolFront());
      poolPop();
      connect_count--;
    }
    if (!isEmpty()) {
      conn = poolFront();
      poolPop();
    } else {
      if (connect_count < MAX_CONNECT)
        conn = createOneConnect(); 
      else 
        std::cerr << "the number of mysql connections is too much!" << std::endl;
    }
  } else {
    if (connect_count < MAX_CONNECT)
      conn = createOneConnect(); 
    else 
      std::cerr << "the number of mysql connections is too much!" << std::endl;
  }
  poollock.unlock();
  return conn;
}
/*
 *将有效的链接对象放回链接池队列中，以待下次的取用。
 */
void MysqlPool::close(MYSQL* conn) {
  if (conn != NULL) {
    poollock.lock();
    mysqlpool.push(conn);
    poollock.unlock();
  }
}
/*
 * sql语句执行函数，并返回结果，没有结果的SQL语句返回空结果，
 * 每次执行SQL语句都会先去连接队列中去一个连接对象，
 * 执行完SQL语句，就把连接对象放回连接池队列中。
 * 返回对象用map主要考虑，用户可以通过数据库字段，直接获得查询的字。
 * 例如：m["字段"][index]。
 */
std::unordered_map<std::string,std::vector<std::string>> MysqlPool::executeSql(const std::string& command){
  MYSQL* conn = getOneConnect();
  mysql_autocommit(conn,1);
  std::unordered_map<std::string,std::vector<std::string> > results;
  if (conn) {
    if (mysql_query(conn,command.c_str()) == 0) {
      MYSQL_RES *res = mysql_store_result(conn);
      if (res) {
        MYSQL_FIELD *field;
        while ((field = mysql_fetch_field(res))) {
          results.insert(make_pair(field->name,std::vector<std::string>()));
        }
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
          unsigned int i = 0;
          for (std::unordered_map<std::string,std::vector<std::string>>::iterator it = results.begin();
              it != results.end(); ++it) {
            (it->second).push_back(std::string(row[i++]));
          }     
        }
        mysql_free_result(res);
      } else {
        if (mysql_field_count(conn) != 0)
          std::cerr << mysql_error(conn) << std::endl;
      }
    } else {
      std::cerr << mysql_error(conn) <<std::endl;
    }
    close(conn);
  } else {
    std::cerr << mysql_error(conn) << std::endl;
  }
  return results;
}

void MysqlPool::selectUser(signaling::Signaling& signal){
  std::unordered_map<std::string,std::vector<std::string>>result=executeSql(signal.text());
  std::unordered_map<std::string,std::vector<std::string>>::iterator it = result.begin();
  unsigned size=it->second.size();
  for(int i=0;i<it->second.size();++i){
    transfer::User user;
    if(result.count("userID"))user.set_userid(atoi(result["userID"][i].c_str()));
    if(result.count("account"))user.set_account(result["account"][i]);
    if(result.count("password"))user.set_password(result["password"][i]);
    if(result.count("status"))user.set_status(result["status"][i]);
    if(result.count("nickname"))user.set_nickname(result["nickname"][i]);
    if(result.count("createTime"))user.set_createtime(result["createTime"][i]);
    transfer::User* userp=signal.add_users();
    *userp=user;
  }
}

void MysqlPool::insertUser(signaling::Signaling& signal){
  for(int i=0;i<signal.users().size();++i){
    std::string sqltext="INSERT INTO user VALUES(DEFAULT,'";
    sqltext+=signal.users()[i].account()+"','";
    sqltext+=signal.users()[i].password()+"',DEFAULT,'";
    sqltext+=signal.users()[i].nickname()+"',DEFAULT)";
    std::unordered_map<std::string,std::vector<std::string>>result=executeSql(sqltext);
    if(result.size()){
      signal.set_command("insertUser");
      signal.set_text("fail");
      return ;
    }
  }
  signal.set_command("insertUser");
  signal.set_text("success");
}

void MysqlPool::updateUser(signaling::Signaling& signal){
  std::unordered_map<std::string,std::vector<std::string>>result=executeSql(signal.text());
  signal.set_command("updateUser");
  if(result.size())signal.set_text("fail");
  else signal.set_text("success");
}
/*INSERT INTO files(fileSize,fileCode,fileName,filePath)
VALUES(123,'0123456789abcdefghij','文件一.txt','pathA/pathB/pathC/');*/

void MysqlPool::insertFile(signaling::Signaling& signal){
  for(int i=0;i<signal.files().size();++i){
    std::string sqltext="INSERT INTO files(fileSize,fileCode,fileName,filePath) VALUES(";
    sqltext+=std::to_string(signal.files()[i].filesize())+",'";
    sqltext+=signal.files()[i].filecode()+"','";
    sqltext+=signal.files()[i].filename()+"','";
    sqltext+=signal.files()[i].filepath()+"');";
    std::unordered_map<std::string,std::vector<std::string>>result=executeSql(sqltext);
    if(result.size()){
      signal.set_command("insertFile");
      signal.set_text("fail");
      return ;
    }
  }
  signal.set_command("insertFile");
  signal.set_text("success");
}

void MysqlPool::execute(signaling::Signaling& signal){
  if(signal.command()=="selectUser")selectUser(signal);
  else if(signal.command()=="insertUser")insertUser(signal);
  else if(signal.command()=="updateUser")updateUser(signal);
  else if(signal.command()=="insertFile")insertFile(signal);
}


/*
 * 析构函数，将连接池队列中的连接全部关闭
 */
MysqlPool::~MysqlPool() {
  while (poolSize() != 0) {
    mysql_close(poolFront());
    poolPop();
    connect_count--;
  }
}



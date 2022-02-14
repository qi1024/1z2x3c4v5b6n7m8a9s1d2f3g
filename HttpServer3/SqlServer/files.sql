-- /home/huawei/project/HttpServer/sql/files.sql


/*DROP DATABASE IF EXISTS transfer;
CREATE DATABASE transfer DEFAULT CHARACTER SET='UTF8';*/


USE transfer;

DROP TABLE IF EXISTS user;
CREATE TABLE user(
	userID		int(10)			AUTO_INCREMENT              COMMENT "用户ID",
    account	    varchar(30)		NOT NULL UNIQUE				COMMENT "邮箱账号",
	password	varchar(20)     NOT NULL					COMMENT "用户密码",
    status	    varchar(20)	    DEFAULT '正常'               COMMENT "账号状态",
    nickname	varchar(15)	    NOT NULL UNIQUE			    COMMENT "用户昵称",
    createTime	timestamp		DEFAULT current_timestamp	COMMENT "创建时间",
    PRIMARY KEY(userID)
)AUTO_INCREMENT=100001;

-- INSERT INTO user VALUES(DEFAULT,'00@00.COM',000000,DEFAULT,'张三',DEFAULT);


DROP TABLE IF EXISTS files;
CREATE TABLE files(
	fileID		int(10)			AUTO_INCREMENT				COMMENT "文件ID",
    fileSize	bigint(15)			NOT NULL					COMMENT "文件大小",
    fileCode	varchar(20)		NOT NULL UNIQUE				COMMENT "文件提取码",
	fileName	varchar(128)	NOT NULL					COMMENT "文件名称",
    filePath	varchar(1024)	NOT NULL					COMMENT "文件路径",
    uploadTime	timestamp		DEFAULT current_timestamp	COMMENT "上传时间",
    PRIMARY KEY(fileID)
)AUTO_INCREMENT=100001;

-- INSERT INTO files(fileSize,fileCode,fileName,filePath)VALUES(123,'0123456789abcdefghij','文件一.txt','pathA/pathB/pathC/');

DROP TABLE IF EXISTS record;
CREATE TABLE record(
	recordID		int(10)			AUTO_INCREMENT			    COMMENT "操作记录",
    account	        varchar(30)		NOT NULL				    COMMENT "账号",
    operation	    varchar(10)		NOT NULL                    COMMENT "操作",
    targetAccount   varchar(30)		DEFAULT '无'				COMMENT "对方账号",
    nickname	    varchar(15)	    DEFAULT '无'				 COMMENT "对方昵称",
	fileName	    varchar(128)	NOT NULL				    COMMENT "文件名称",
    fileSize	    bigint(10)			NOT NULL					COMMENT "文件大小",
    fileCode	    varchar(20)		DEFAULT '无'				 COMMENT "文件提取码",
    operateTime	    timestamp		DEFAULT current_timestamp	COMMENT "操作时间",
    PRIMARY KEY(recordID),
    foreign key(account) references user(account) on delete cascade on update cascade
)AUTO_INCREMENT=100001;

/*
INSERT INTO files(fileSize,fileCode,fileName,filePath)
	VALUES(123,'0123456789abcdefghij','文件一.txt','pathA/pathB/pathC/');
    
INSERT INTO files(fileSize,fileCode,fileName,filePath)
	VALUES(4567,'1023456789abcdefghij','文件二.mp3','pathA/pathB/pathC/');
    
INSERT INTO files(fileSize,fileCode,fileName,filePath)
	VALUES(12345,'2103456789abcdefghij','文件三.flv','pathA/pathB/pathC/');
    */
    
    /*定时任务*/
	-- set global event_scheduler =1;
    -- DROP EVENT IF EXISTS delete_file;
    -- create event delete_file on schedule every 24*3*60*60 second do 
    -- delete from files where  timediff(now(),uploadTime)>=24*3*60*60; 
    -- DROP EVENT delete_file;








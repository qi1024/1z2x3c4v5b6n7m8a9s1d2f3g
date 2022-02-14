#pragma once

#include <sstream>
#include <fstream>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cassert>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/epoll.h>




#include "../tools/email.hpp"
#include "../sql/mysql_service.hpp"
#include "../tools/newrand.hpp"
#include "../tools/log.hpp"
#include "../projo/RequestHeader.hpp"
#include "../projo/ResponseHeader.hpp"
#include "../projo/RequestHeader.hpp"
#include "../projo/WsRequestHeader.hpp"
#include "../projo/WsResponseHeader.hpp"
#include "../projo/users.hpp"
#include "../projo/wsconnect.hpp"
#include "encrypt.hpp"



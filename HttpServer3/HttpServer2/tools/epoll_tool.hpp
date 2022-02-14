#pragma once
#include <fcntl.h>

namespace epoll {
	int setNonblocking(int fd) {
		int old_option = fcntl(fd, F_GETFL);
		int new_option = old_option | O_NONBLOCK;
		fcntl(fd, F_SETFL, new_option);
		return old_option;
	}

	void addfd(int epollfd, int fd, bool one_shot=false) {
		epoll_event event;
		event.data.fd = fd;
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;//(连接到达有数据来临 | 边缘出发 | 对方中断连接)
		//event.events = EPOLLIN | EPOLLRDHUP;//(连接到达有数据来临 | 边缘出发 | 对方中断连接)
		if (one_shot)event.events |= EPOLLONESHOT;
		epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
		setNonblocking(fd);
	}

	void removefd(int epollfd, int fd) {
		epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
		close(fd);
	}

	void modfd(int epollfd, int fd, int ev) {
		epoll_event event;
		event.data.fd = fd;
		event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
		epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
	}
}


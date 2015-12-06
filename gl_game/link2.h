#ifndef _LINK2_H_
#define _LINK2_H_

#include "main.h"

#define L2_LEFT 0
#define L2_RIGHT 1
#define L2_UP 2
#define L2_DOWN 3

int opposite_dir(const int& dir) {
	switch (dir) {
	case L2_LEFT:	return L2_RIGHT;
	case L2_RIGHT:	return L2_LEFT;
	case L2_UP:		return L2_DOWN;
	case L2_DOWN:	return L2_UP;
	}
}

template<class T>
class Link2 {
public:
	T *data;
	Link2<T> *adj[4];//LRUD

	Link2<T>() {
		for (int i = 0; i < 4; i++)
			adj[i] = NULL;
		data = NULL;
	}

	Link2<T>(const T& _data) : Link2<T>() {
		data = new T(_data);
	}

	~Link2<T>() {
		if (data != NULL) delete data;
		for (int i = 0; i < 4; i++) {
			if (adj[i] != NULL)
				adj[i]->adj[(i/2)*2 + (i%2 == 0 ? 1 : 0)];
				delete adj[i];
		}
	}

	Link2<T> *find(const int& i, const int& j) {
		if (i == 0) {
			if (j == 0)
				return this;
			else
				return adj[L2_DOWN]->find(i, j - 1);
		}
		else
			return adj[L2_RIGHT]->find(i - 1, j);
	}

	void connect(const int& dir, Link2<T> *ptr) {
		adj[dir] = ptr;
		ptr->adj[opposite_dir(dir)] = this;
	}

	void insert(const int& dir, const T& data) {
		if (adj[dir] == NULL)
			connect(dir, new Link2<T>(data));
		else {
			Link2<T> *temp = adj[dir];
			connect(dir, new Link2<T>(data));
			adj[dir]->connect(dir, temp);
			temp = NULL;
		}
	}

	int get_width() {
		if (adj[L2_RIGHT] == NULL)
			return 1;
		else
			return 1 + adj[L2_RIGHT]->get_width();
	}

	int get_depth() {
		if (adj[L2_DOWN] == NULL)
			return 1;
		else
			return 1 + adj[L2_DOWN]->get_width();
	}

	void insert_col(const T& _data) {
		Link2<T> *trace = this;
		while (trace != NULL) {
			trace->insert(L2_RIGHT, _data);
			if (trace->adj[L2_UP] != NULL)
				trace->adj[L2_RIGHT]->connect(L2_UP, trace->adj[L2_UP]->adj[L2_RIGHT]);
			trace = trace->adj[L2_DOWN];
		}
	}
	void insert_row(const T& _data) {
		Link2<T> *trace = this;
		while (trace != NULL) {
			trace->insert(L2_DOWN, _data);
			if (trace->adj[L2_LEFT] != NULL)
				trace->adj[L2_DOWN]->connect(L2_LEFT, trace->adj[L2_LEFT]->adj[L2_DOWN]);
			trace = trace->adj[L2_RIGHT];
		}
	}
};

#endif
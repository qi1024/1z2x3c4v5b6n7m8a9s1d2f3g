/*#include<iostream>
#include<cassert>
#include<ctime>
#include<vector>

#include "memory_pool.hpp"

#define ELEMS 100000		//����Ԫ�صĸ���
#define REPS 100			//�ظ�����

int main() {
	int a = 5;
	clock_t start;

	//ʹ��STLĬ�ϵķ�����
	StackAlloc<int, std::allocator<int> >stackDefault;
	start = clock();
	for (int i = 0; i < REPS; ++i) {
		assert(stackDefault.empty());
		for (int j = 0; j < ELEMS; ++j)stackDefault.push(j);
		for (int j = 0; j < ELEMS; ++j)stackDefault.pop();
	}
	std::cout << "Default Allocator Time:";
	std::cout << (((double)clock()-start)/CLOCKS_PER_SEC) << std::endl;

	//ʹ���ڴ��
	StackAlloc<int, std::allocator<int> >stackPool;
	start = clock();
	for (int i = 0; i < REPS; ++i) {
		assert(stackPool.empty());
		for (int j = 0; j < ELEMS; ++j)stackPool.push(j);
		for (int j = 0; j < ELEMS; ++j)stackPool.pop();
	}
	std::cout << "MemoryPool Allocator Time:";
	std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << std::endl;

	std::cin.get();
	return 0;
}
*/


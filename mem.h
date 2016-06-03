// mem.h
#ifndef __MEM_H__
#define __MEM_H__

#include <memory>

void * operator new(std::size_t size) throw(std::bad_alloc);
void operator delete(void * mem) throw();

#endif // __MEM_H__

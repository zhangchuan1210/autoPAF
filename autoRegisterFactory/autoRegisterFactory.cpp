// autoRegisterFactory.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
/*
可变参数资助注册工厂
*/

#include "pch.h"
#include <iostream>
#include<functional>
#include<memory>
#include<map>
#include <tuple>
#include <cassert>
struct message_base;
template<typename... Args>
class MsgFactory {
public:
	MsgFactory() = delete;
	typedef std::function<std::unique_ptr<message_base>(std::tuple<Args...>)> CreateMethod;
	static bool register_(size_t id,CreateMethod createMethod) {
		auto& map = getRegisterMap();
		auto it = map.find(id);
		if (it==map.end()) {
			map[id]=createMethod;
			std::cout<<"register success: "+id<<std::endl;
			return true;
		}
		return false;
	}
	static std::unique_ptr<message_base> create_(size_t id,Args... a1) {
		auto& map = getRegisterMap();
		auto it = map.find(id);
	
		if (it!=map.end()) 
			return it->second(std::make_tuple(a1...));
		
		return nullptr;
	}
	static std::map<size_t,CreateMethod>& getRegisterMap() {
		static std::map<size_t, CreateMethod> registerMap;
		return registerMap;
	}


};
template<typename t,typename... Args>
class AutoRegisterFactory {
protected:
	static bool register1;
};
template<typename t, typename... Args>
std::unique_ptr<t>  create(std::tuple<Args...> args)
{
	/*return std::apply([](Args... a)
	{
		return std::make_unique<t>(a...);
	}, args);*/
	return std::make_unique<t>(args);
}


template<typename t, typename... Args>
bool AutoRegisterFactory<t,Args...>::register1= MsgFactory<Args...>::register_(t::Id(),[](std::tuple<Args...> args)
{
	return create<t>(args);

} );



struct message_base
{
public:
	virtual ~message_base() {};

};
struct message1:public message_base,AutoRegisterFactory<message1,int>
{
	message1(int) {}
	static size_t Id()
	{
		assert(register1);
		return 1;
	}
};

int main()
{
	auto msg = MsgFactory<int>::create_(1,1);
	
	std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

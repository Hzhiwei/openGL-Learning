#pragma once


namespace HGLTool
{
	/*
	此类用于引用计数
	某个类的对象在新建一个资源后，但在此对象被析构时，由于新建的资源还要给
	其他对象使用，因此，不能在析构函数中直接释放资源。
	此时可在类中包含此类的一个指针C：
	当直接生成资源时（直接新建资源的构造函数，其他加载资源的函数等），将C 
	new一个此对象，并Increase()
	当从其他地方共享资源时（operator=，拷贝构造函数，复制函数等），原先的C->Decrease()
	返回true即无其他对象使用资源，即可释放资源，delete C，将此对象的C 指向
	资源来源的对象的 C
	析构函数中，C->Decrease()，true即无其他对象使用资源，即可释放资源，delete C，
	否则保留资源和C

	引用计数构造对象默认为0

	Increase()：引用计数加一
	Decrease()：引用计数减一，结果为0返回true，非0返回false
	*/
	class HGLReferenceCounter
	{
	public:
		void Increase()
		{
			++Num;
		}

		bool Decrease()
		{
			--Num;
			if (Num == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool IsOnlyOne()
		{
			if (Num == 1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		unsigned int ReferenceNum()
		{
			return Num;
		}

	private:
		unsigned int Num = 0;
	};
}



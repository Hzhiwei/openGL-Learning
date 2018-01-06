#pragma once


namespace HGLTool
{
	/*
	�����������ü���
	ĳ����Ķ������½�һ����Դ�󣬵��ڴ˶�������ʱ�������½�����Դ��Ҫ��
	��������ʹ�ã���ˣ�����������������ֱ���ͷ���Դ��
	��ʱ�������а��������һ��ָ��C��
	��ֱ��������Դʱ��ֱ���½���Դ�Ĺ��캯��������������Դ�ĺ����ȣ�����C 
	newһ���˶��󣬲�Increase()
	���������ط�������Դʱ��operator=���������캯�������ƺ����ȣ���ԭ�ȵ�C->Decrease()
	����true������������ʹ����Դ�������ͷ���Դ��delete C�����˶����C ָ��
	��Դ��Դ�Ķ���� C
	���������У�C->Decrease()��true������������ʹ����Դ�������ͷ���Դ��delete C��
	��������Դ��C

	���ü����������Ĭ��Ϊ0

	Increase()�����ü�����һ
	Decrease()�����ü�����һ�����Ϊ0����true����0����false
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



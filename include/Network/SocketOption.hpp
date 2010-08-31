#ifndef __SOCKET_SOCKET_OPTION_HPP
#define __SOCKET_SOCKET_OPTION_HPP



namespace async
{
	namespace network
	{




		// ----------------------------------------------
		// ����bool����socket���Եİ�����

		template<int _Level, int _Name>
		class Boolean
		{
		private:
			int value_;

		public:
			Boolean()
				: value_(0)
			{}
			explicit Boolean(bool val)
				: value_(val ? 1 : 0)
			{}
			Boolean &operator=(const Boolean &rhs)
			{
				value_	 = rhs.value_;
				return *this;
			}	
			Boolean &operator=(bool val)
			{
				value_ = val ? 1 : 0;
			}

		public:
			bool value() const
			{
				return !!value_;
			}

			// bool ת��
			operator bool() const
			{
				return !!value_;
			}

			bool operator!() const
			{
				return !value_;
			}

			// ��ȡSocket levalѡ��ֵ
			int level() const
			{
				return _Level;
			}

			// ��ȡSocket name
			int name() const
			{
				return _Name;
			}	

			// ��ȡSocketֵ
			char *data()
			{
				return &value_;
			}
			const char *data() const
			{
				return &value_;
			}

			// ��ȡֵ��С
			size_t size() const
			{
				return sizeof(value_);
			}


			// ����boolean�Ĵ�С
			void resize(size_t nSize)
			{
				// ��ĳЩƽ̨��getsockopt����һ��sizeof(bool)--1�ֽڡ�	
				// 
				switch(nSize)
				{
				case sizeof(char):
					value_ = *reinterpret_cast<char *>(&value_) ? 1 : 0;
					break;
				case sizeof(value_):
					break;
				default:
					throw std::length_error("bool socket opetion resize");
				}
			}
		};


		// ----------------------------------------------------------
		// ����int����socket���Եİ�����

		template<int _Level, int _Name>
		class Integer
		{
		private:
			int value_;

		public:
			Integer()
				: value_(0)
			{}
			explicit Integer(int val)
				: value_(val)
			{}
			Integer &operator=(const Integer &rhs)
			{
				value_	 = rhs.value_;
				return *this;
			}	
			Integer &operator=(int val)
			{
				value_ = val;
			}

		public:
			bool value() const
			{
				return !!value_;
			}

			// ��ȡSocket levalѡ��ֵ
			int level() const
			{
				return _Level;
			}

			// ��ȡSocket name
			int name() const
			{
				return _Name;
			}	

			// ��ȡSocketֵ
			char *data()
			{
				return reinterpret_cast<char *>(&value_);
			}
			const char *data() const
			{
				return (const char *)&value_;
			}

			// ��ȡֵ��С
			size_t size() const
			{
				return sizeof(value_);
			}


			// ����int�Ĵ�С
			void resize(size_t nSize)
			{
				if( nSize != sizeof(value_) )
					throw std::length_error("Integer socket option resize");
			}
		};


		// -----------------------------------------------------------------
		// class Linger ѡ�����ð�����

		// SO_LINGER���ڿ��Ƶ�δ���͵��������׽������Ŷ�ʱ��һ��ִ����closesocket�󣬸ò�ȡ���ֶ���
		// ���������ʱ����δ���ͺͽ��ܵ����ݶ��ᶪ����ͬʱ����Է�������
		// Ҳ����ͨ������SO_DONTLINGER������

		template<int _Level, int _Name>
		class Linger
		{
		private:
			::linger value_;


		public:
			Linger()
			{
				value_.l_linger = 0;	// option on/off
				value_.l_onoff	= 0;	// linger time
			}
			Linger(bool bOn, int nTime)
			{
				enabled(bOn);
				timeout(nTime);
			}


		public:
			// �򿪻�ر�lingerѡ��
			void enabled(bool val)
			{
				value_.l_onoff = val ? 1 : 0;
			}

			bool enabled() const
			{
				return value_.l_onoff != 0;
			}

			void timeout(int val)
			{
				value_.l_linger = static_cast<u_short>(val);
			}
			int timeout() const
			{
				return static_cast<int>(value_.l_linger);
			}

			// ��ȡSocket levalѡ��ֵ
			int level() const
			{
				return _Level;
			}

			// ��ȡSocket name
			int name() const
			{
				return _Name;
			}	

			// ��ȡSocketֵ
			::linger *data()
			{
				return &value_;
			}
			const ::linger *data() const
			{
				return &value_;
			}

			// ��ȡֵ��С
			size_t size() const
			{
				return sizeof(value_);
			}

			// ����int�Ĵ�С
			void resize(size_t nSize)
			{
				if( nSize != sizeof(value_) )
					throw std::length_error("Linger socket option resize");
			}

		};






		typedef u_long IoctlArgType;

		// ---------------------------------------------------
		// class NonBlockingIO

		class NonBlockingIO
		{
		private:
			IoctlArgType value_;

		public:
			NonBlockingIO()
				: value_(0)
			{}
			NonBlockingIO(bool value)
				: value_(value ? 1 : 0)
			{}

		public:
			int name() const
			{
				return FIONBIO;
			}

			void set(bool value)
			{
				value_ = value ? 1 : 0;
			}
			bool get() const
			{
				return value_ != 0;
			}

			IoctlArgType *data()
			{
				return &value_;
			}

			const IoctlArgType *data() const
			{
				return &value_;
			}
		};


		// ---------------------------------------------------
		// class BytesReadable

		class BytesReadable
		{
		private:
			IoctlArgType value_;

		public:
			BytesReadable()
				: value_(0)
			{}
			BytesReadable(bool value)
				: value_(value ? 1 : 0)
			{}

		public:
			int name() const
			{
				return FIONREAD;
			}

			void set(size_t value)
			{
				value_ = static_cast<IoctlArgType>(value);
			}
			size_t get() const
			{
				return static_cast<size_t>(value_);
			}

			IoctlArgType *data()
			{
				return &value_;
			}

			const IoctlArgType *data() const
			{
				return &value_;
			}
		};


		//----------------------------------

		/*
			Example:

				tcp::socket sock(io);
				SocketBase::Broadcast opt(true);
				socket.SetOption(opt);
			*/

			typedef Boolean<SOL_SOCKET, SO_BROADCAST>	Broadcast;
			typedef Boolean<SOL_SOCKET, SO_DEBUG>		Debug;
			typedef Boolean<SOL_SOCKET, SO_DONTROUTE>	DontRoute;
			typedef Boolean<SOL_SOCKET, SO_KEEPALIVE>	KeepAlive;
			typedef Boolean<SOL_SOCKET, SO_REUSEADDR>	ReuseAddr;

			typedef Integer<SOL_SOCKET, SO_SNDBUF>		SendBufSize;
			typedef Integer<SOL_SOCKET, SO_SNDLOWAT>	SendLowWaterMark;
			typedef Integer<SOL_SOCKET, SO_RCVBUF>		RecvBufSize;
			typedef Integer<SOL_SOCKET, SO_RCVLOWAT>	RecvLowWwaterMark;
			//typedef Integer<SOL_SOCKET, SO_LINGER>		Linger;
			typedef Integer<SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT> UpdateAcceptContext;


			typedef NonBlockingIO						NonBlockingIO;
			typedef BytesReadable						BytesReadable;
	}
}





#endif
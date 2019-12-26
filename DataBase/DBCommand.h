#pragma once
namespace Util
{
	namespace DB
	{
		class DBConnection;
		class DBResultSet;
		class DBCommand abstract
		{
			DBConnection*	_pConnection;
			std::string		_Query;
			DBResultSet*	_pResultSet;
		public:
			DBCommand(DBConnection* const pConnection, const std::string& Query);
			virtual ~DBCommand();

			void Prepare();
			void Reset();

			virtual DBResultSet* Execute() = 0;
			virtual void Close() = 0;

			virtual void Bind(const uint8_t& value) = 0;
			virtual void Bind(const uint16_t& value) = 0;
			virtual void Bind(const uint32_t& value) = 0;
			virtual void Bind(const uint64_t& value) = 0;
			virtual void Bind(const int8_t& value) = 0;
			virtual void Bind(const int16_t& value) = 0;
			virtual void Bind(const int32_t& value) = 0;
			virtual void Bind(const int64_t& value) = 0;
			virtual void Bind(char* value, const uint32_t length) = 0;

		protected:
			virtual void PrepareImpl() = 0;
			virtual DBResultSet* NewResultSetImpl() = 0;
			virtual void ResetImpl() = 0;
		};

	}
}

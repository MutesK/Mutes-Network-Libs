#pragma once


using namespace std;
/*
Object Pool (FreeList����)
�����̴� �˰����� ��ü�� Stack�� �����.
Lock Free �˰������� ȿ������ ����� ���ü� ���� ����� ���� ���� ����� �����ϱ� ��ƴ�.
������ Lock Free �˰������ Fast Spin Lock �˰����� ���� �򰡿��� Spin Lock�� �̱�� �ణ �����.
���� Lock Free �˰������� ���װ� ������ ��� �����.
�� ���� ���Ҳ��� �Ⱦ��°� ����.
std::shared_mutex �� OwnChecking ����� ���� SafeMutex�� ����� ���Կ���.
���� ���ǻ���
* MEMORYPOOL_CALL_CTOR �÷��׸� ������̶��, �⺻ �����ڴ� ������ �����ؾߵȴ�.
*/

#define VALIDCODE (0x77777777)

template <class NodeType>
class CObjectPool
{
private:
	struct BLOCKNODE
	{
		NodeType Data;
		uint64_t ValidCode;
		uint32_t allocindex;

		BLOCKNODE(uint32_t allocindex)
		{
			ValidCode = VALIDCODE;
			this->allocindex = allocindex;
		}

		~BLOCKNODE()
		{
			cout << "Bug Detected Shared Pointer Reference Count is Zero\n";
			*((int *)0) = 1;
		}
	};
public:
	CObjectPool(int blockSize = 10000, int maxBlockSize = 10000, bool flagcallctor = false);
	virtual ~CObjectPool();

	NodeType* Alloc(void);
	bool Free(NodeType* Data);

	int GetBlockCount(void);
	int GetAllocCount(void);
private:
	bool _isUsector;

	std::atomic<uint32_t>  _allocCount;
	std::atomic<uint32_t>  _maxBlockSize;
	std::atomic<uint32_t>  _blockSize;

	std::map<uint32_t, BLOCKNODE *>		 _freelist;
	std::stack<uint32_t>				 _freestack;  // �Ⱦ��� �ε��� ��Ƴ��� �ڷᱸ�� 

	std::mutex				     _lock;
};



template <class NodeType>
CObjectPool<NodeType>::CObjectPool(int blockSize, int maxBlockSize, bool flagcallctor)
{
	_allocCount = 0;
	_maxBlockSize = 0;
	_blockSize = 0;
	_freelist.clear();

	_isUsector = false;

	for (int i = 0; i < blockSize; i++)
	{
		_freelist[i] = static_cast<BLOCKNODE *>(malloc(sizeof(BLOCKNODE)));
		_freelist[i]->allocindex = i;
		_freelist[i]->ValidCode = VALIDCODE;
		_freestack.push(i);
	}

	_isUsector = flagcallctor;
	_maxBlockSize = maxBlockSize;
	_blockSize = blockSize;
}
template <class NodeType>
CObjectPool<NodeType>::~CObjectPool()
{
	for (auto iter = _freelist.begin(); iter != _freelist.end(); ++iter)
	{
		std::free(iter->second);
	}

	_freelist.clear();
}
template <class NodeType>
NodeType* CObjectPool<NodeType>::Alloc(void)
{
	std::lock_guard<std::mutex> guard(_lock);
	if (_allocCount >= _blockSize)
	{
		// ���� DATA�� �����ߵ�.
		if (_maxBlockSize <= _blockSize)
			return nullptr;

		int makedblocksize = 2 * _blockSize;
		
		for (int i = _blockSize; i < makedblocksize; i++)
		{
			_freelist[i] = static_cast<BLOCKNODE *>(malloc(sizeof(BLOCKNODE)));
			_freelist[i]->allocindex = i;
			_freelist[i]->ValidCode = VALIDCODE;
			_freestack.push(i);
		}
		_blockSize = makedblocksize;
	}

	int allocindex = static_cast<int>(_freestack.top());
	_freestack.pop();

	NodeType* ret = &_freelist[allocindex]->Data;
	++_allocCount;


#ifdef MEMORYPOOL_CALL_CTOR
	if (_isUsector)
	{
		new (ret) NodeType();
	}
#endif

	return ret;
}
template <class NodeType>
bool CObjectPool<NodeType>::Free(NodeType* Data)
{
	std::lock_guard<std::mutex> guard(_lock);

	if (_allocCount <= 0)
		return false;

	// Danger Code
	BLOCKNODE* pBlockNode = reinterpret_cast<BLOCKNODE *>(Data);

	if (pBlockNode->ValidCode != VALIDCODE)
		return false;

	uint32_t allocindex = pBlockNode->allocindex;
	_freestack.push(allocindex);
	--_allocCount;

#ifdef MEMORYPOOL_CALL_CTOR
	if (_isUsector)
	{
		Data->~NodeType();
	}
#endif

	return true;
}

template <class NodeType>
int CObjectPool<NodeType>::GetBlockCount(void)
{
	return _blockSize;
}

template <class NodeType>
int CObjectPool<NodeType>::GetAllocCount(void)
{
	return _allocCount;
}
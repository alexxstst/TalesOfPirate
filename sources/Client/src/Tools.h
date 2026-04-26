#pragma once

std::string ConvertNumToChinese(int num);

template <class T>
class CLimit {
public:
	CLimit(T max, T min = 0) : _max(max), _min(min) {
		_cur = min;
	}

	CLimit() : _max(0), _min(0), _cur(0) {
	}

	CLimit& operator=(T v) {
		SetCur(v);
	}

	CLimit& operator++() {
		++_cur;
		if (_cur > _max) _cur = _max;
		return *this;
	}

	CLimit operator++(int n) {
		CLimit tmp = *this;
		++(*this);
		return tmp;
	}

	CLimit& operator--() {
		--_cur;
		if (_cur < _min) _cur = _min;
		return *this;
	}

	CLimit operator--(int n) {
		CLimit tmp = *this;
		--(*this);
		return tmp;
	}

	bool SetCur(T v) {
		if (v >= _min && v <= _max) {
			_cur = v;
			return true;
		}
		return false;
	}

	bool SetRange(T max, T min = 0) {
		if (min <= max) {
			_min = min;
			_max = max;
			_cur = _min;
		}
		return false;
	}

	T GetMin() {
		return _min;
	}

	T GetMax() {
		return _max;
	}

	T GetCur() {
		return _cur;
	}

private:
	T _min;
	T _max;
	T _cur;
};

template <class T>
class CSizeArray {
public:
	CSizeArray() : _pChange(NULL), _nCount(0) {
	};

	CSizeArray(const CSizeArray& rhs) : _pChange(NULL), _nCount(0) {
		SetValue(rhs._pChange, rhs._nCount);
	}

	CSizeArray& operator=(const CSizeArray& rhs) {
		SetValue(rhs._pChange, rhs._nCount);
		return *this;
	}

	~CSizeArray() {
		if (_pChange) delete [] _pChange;
	}

	T& operator[](int n) {
		return _pChange[n];
	}

	void Resize(unsigned int nCount) {
		_Create(nCount);
	}

	unsigned int GetCount() {
		return _nCount;
	}

	T* GetValue() {
		return _pChange;
	}

	bool IsEmpty() {
		return _nCount == 0;
	}

	void SetValue(T* pChange, unsigned int nCount) {
		_Create(nCount);
		memcpy(_pChange, pChange, sizeof(T) * nCount);
	}

private:
	void _Create(unsigned int nCount) {
		{
			if (nCount == 0) {
				if (_pChange) {
					delete [] _pChange;
					_pChange = NULL;
					_nCount = 0;
				}
				return;
			}

			if (_pChange) {
				if (_nCount != nCount) {
					delete [] _pChange;
					_pChange = new T[nCount];
				}
			}
			else {
				_pChange = new T[nCount];
			}
			_nCount = nCount;
		}
	}

private:
	T* _pChange;
	unsigned int _nCount;
};

class CTimeWork {
public:
	CTimeWork(DWORD sleep = 1000) : dwTime(0), dwSleep(sleep) {
	}

	bool IsTimeOut(DWORD time) {
		if (time - dwTime > dwSleep) {
			dwTime = time;
			return true;
		}
		return false;
	}

	void SetSleep(DWORD v) {
		dwSleep = v;
	}

	DWORD GetSleep() {
		return dwSleep;
	}

private:
	DWORD dwTime;
	DWORD dwSleep;
};

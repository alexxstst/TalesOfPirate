//----------------------------------------------------------------------
// :
// :lh 2004-07-19
// :
// :2004-10-26
//----------------------------------------------------------------------
#pragma once

namespace GUI {
	class CStep {
	public:
		CStep()
			: _fMax(10), _fMin(0), _fPosition(0), fStep(1), fLen(10) {
		}

		float GetMax() {
			return _fMax;
		}

		float GetMin() {
			return _fMin;
		}

		bool Add();
		bool Add(float v);
		bool SetPosition(float v);
		bool Sub();
		bool Sub(float v);
		bool SetRange(float min, float max);

		float GetRange() {
			return _fMax - _fMin;
		}

		float GetRate() {
			return _fPosition / (_fMax - _fMin);
		}

		float GetPosition() {
			return _fPosition;
		}

		int GetShowPosition() {
			return (int)(_fPosition / (_fMax - _fMin) * fLen);
		}

		bool SetShowPosition(float fPos);

		float fStep; // 
		float fLen; // 

	private:
		float _fMax;
		float _fMin;
		float _fPosition;
	};

	// 
	inline bool CStep::Add() {
		if (_fPosition < _fMax) {
			_fPosition += fStep;
			if (_fPosition > _fMax) _fPosition = _fMax;
			return true;
		}
		return false;
	}

	inline bool CStep::Add(float v) {
		if (_fPosition < _fMax) {
			_fPosition += (float)v;
			if (_fPosition > _fMax) _fPosition = _fMax;
			return true;
		}
		return false;
	}

	inline bool CStep::SetPosition(float v) {
		if (v != _fPosition) {
			_fPosition = v;
			if (_fPosition > _fMax) _fPosition = _fMax;
			if (_fPosition < _fMin) _fPosition = _fMin;
			return true;
		}
		return false;
	}

	inline bool CStep::Sub() {
		if (_fPosition > _fMin) {
			_fPosition -= fStep;
			if (_fPosition < _fMin) _fPosition = _fMin;

			return true;
		}
		return false;
	}

	inline bool CStep::Sub(float v) {
		if (_fPosition > _fMin) {
			_fPosition -= v;
			if (_fPosition < _fMin) _fPosition = _fMin;

			return true;
		}
		return false;
	}

	inline bool CStep::SetRange(float min, float max) {
		if (min > max) max = min;
		_fMin = min;
		_fMax = max;

		if (_fPosition < _fMin) _fPosition = _fMin;
		if (_fPosition > _fMax) _fPosition = _fMax;

		return true;
	}

	inline bool CStep::SetShowPosition(float fPos) {
		if (fLen < 0.0f) return false;

		if (fPos < 0.0f) fPos = 0.0f;
		else if (fPos > fLen) fPos = fLen;

		_fPosition = fPos / fLen * (_fMax - _fMin);
		return true;
	}
}

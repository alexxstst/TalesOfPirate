//--------------------------------------------------------------
// :
// :lh 2004-11-01
// :false
//--------------------------------------------------------------

#pragma once

class CBoolSet
{
public:
	CBoolSet() = default;

	// bit0~31
	bool IsTrue( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}
	bool IsFalse( unsigned int bit )    { return (_value & 0x00000001 << bit)==0;	}
	void SetTrue( unsigned int bit )	{ _value |= 0x00000001 << bit;				}	// true
	void SetFalse( unsigned int bit )	{ _value &= ~(0x00000001 << bit);			}	// false

	void Set( unsigned int bit, bool v ){ if(v) SetTrue(bit); else SetFalse(bit);	}
	bool Get( unsigned int bit )		{ return (_value & 0x00000001 << bit)!=0;	}

	void AllFalse()						{ _value = 0;			}	// false
	void AllTrue()						{ _value = 0xffffffff;	}	// true

	bool IsNone()						{ return _value==0;		}	// false
	bool IsAny()						{ return _value!=0;		}	// true

private:
	uint32_t _value{ 0 };
};

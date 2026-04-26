#include "stdafx.h"
#include "soundcommon.h"
#include <direct.h>

using namespace std;

string StringToLower(const string& theString) {
	string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += tolower(theString[i]);

	return aString;
}

string GetDirectSoundString(HRESULT theResult) {
	switch (theResult) {
	case DSERR_ALLOCATED: return "DSERR_ALLOCATED";
	case DSERR_ALREADYINITIALIZED: return "DSERR_ALREADYINITIALIZED";
	case DSERR_BADFORMAT: return "DSERR_BADFORMAT";
	case DSERR_BUFFERLOST: return "DSERR_BUFFERLOST";
	case DSERR_CONTROLUNAVAIL: return "DSERR_CONTROLUNAVAIL";
	case DSERR_GENERIC: return "DSERR_GENERIC";
	case DSERR_INVALIDCALL: return "DSERR_INVALIDCALL";
	case DSERR_NOAGGREGATION: return "DSERR_NOAGGREGATION";
	case DSERR_NODRIVER: return "DSERR_NODRIVER";
	case DSERR_NOINTERFACE: return "DSERR_NOINTERFACE";
	case DSERR_OTHERAPPHASPRIO: return "DSERR_OTHERAPPHASPRIO";
	case DSERR_PRIOLEVELNEEDED: return "DSERR_PRIOLEVELNEEDED";
	case DSERR_UNINITIALIZED: return "DSERR_UNINITIALIZED";

	case E_OUTOFMEMORY: return "E_OUTOFMEMORY";
	}

	return std::format("{:x}", static_cast<unsigned int>(theResult));
}

void MkDir(const string& theDir) {
	string aPath = theDir;

	int aCurPos = 0;
	for (;;) {
		auto aSlashPos = aPath.find('\\', aCurPos);
		if (aSlashPos == std::string::npos) {
			mkdir(aPath.c_str());
			break;
		}

		aCurPos = aSlashPos + 1;

		string aCurPath = aPath.substr(0, aSlashPos);
		mkdir(aCurPath.c_str());
	}
}

string GetFileDir(const string& thePath, bool withSlash) {
	int aLastSlash = max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (aLastSlash == -1)
		return "";
	else {
		if (withSlash)
			return thePath.substr(0, aLastSlash + 1);
		else
			return thePath.substr(0, aLastSlash);
	}
}

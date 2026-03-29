#ifndef IMUSICTHREAD_H
#define IMUSICTHREAD_H

enum EThreadPriority
{
	TPri_Normal,
	TPri_AboveNormal,
	TPri_BelowNormal
};

class IMusicThread
{
public:
	virtual void	SetThreadPriority( EThreadPriority NewPriority ) = 0;
	virtual bool	Begin() = 0;
	virtual bool	Resume() = 0;
	virtual bool	Suspend() = 0;
	virtual bool	Terminate() = 0;
	virtual void	Play( unsigned long MusicID, bool Loop = false ) = 0;
};

extern IMusicThread* GMusicThread;

#endif	//	IMUSICTHREAD_H

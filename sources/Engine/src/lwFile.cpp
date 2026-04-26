//
#include "stdafx.h"

#include "lwFile.h"

LW_BEGIN

//lwFile
DWORD lwFile::__dummy = 0;

LW_STD_IMPLEMENTATION(lwFile)

LW_RESULT lwFile::_CheckDirectory(const char* path)
{
    DWORD a = GetFileAttributes(path);

    if(a == INVALID_FILE_ATTRIBUTES)
        return LW_RET_FAILED;

    return (a & FILE_ATTRIBUTE_DIRECTORY) ? LW_RET_OK : LW_RET_FAILED;
}

LW_RESULT lwFile::CreateDirectory(const char* path, LPSECURITY_ATTRIBUTES attr)
{
    char buf[_MAX_PATH];
    char* ptr = buf;

    _tcscpy(buf, path);

    do
    {
        ptr = _tcschr(ptr, '\\');

        if(ptr != NULL)
            *ptr = '\0';

        if(LW_FAILED(_CheckDirectory(buf)))
        {
            if(::CreateDirectory(buf, attr) == 0)
            {
                ToLogService("errors", LogLevel::Error,
                             "[{}] ::CreateDirectory failed: buf={}, gle={}",
                             __FUNCTION__, buf, static_cast<std::uint32_t>(::GetLastError()));
                return LW_RET_FAILED;
            }
        }

        if(ptr == NULL)
            break;

        *ptr = '\\';
        ptr += 1;

    } while(1);

    return LW_RET_OK;
}
//
LW_RESULT lwFile::CreateFile(const char* file, DWORD access_flag, DWORD share_mode, LPSECURITY_ATTRIBUTES secu_attr, DWORD creation_flag, DWORD attributes_flag)
{
    LW_RESULT ret = LW_RET_FAILED;

    char* ptr;
    char path[_MAX_PATH];
    _tcscpy(path, file);
    
    if(access_flag == GENERIC_WRITE)
    {
        if((ptr = _tcsrchr(path, '\\')))
        {
            *ptr = '\0';

            if(LW_FAILED(_CheckDirectory(path)))
            {
                if(LW_RESULT r = CreateDirectory(path, 0); LW_FAILED(r))
                {
                    ToLogService("errors", LogLevel::Error,
                                 "[{}] CreateDirectory failed: path={}, ret={}",
                                 __FUNCTION__, path, static_cast<long long>(r));
                    goto __ret;
                }
            }
        }
    }

    _handle = ::CreateFile(file, access_flag, share_mode, secu_attr, creation_flag, attributes_flag, 0);

    if(_handle == INVALID_HANDLE_VALUE)
        goto __ret;

    _tcscpy(_file, file);


    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwFile::LoadFileBuffer(const char* file, lwIBuffer* buf)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD in_size;
    DWORD out_size;
    DWORD access_flag;
    DWORD creation_flag;
    DWORD attributes_flag;

    access_flag = GENERIC_READ;
    creation_flag = OPEN_EXISTING;
    attributes_flag = FILE_FLAG_SEQUENTIAL_SCAN;

    if(file == 0 || buf == 0)
        goto __ret;

    if(LW_RESULT r = CreateFile(file, access_flag, FILE_SHARE_READ, 0, creation_flag, attributes_flag); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateFile failed: file={}, ret={}",
                     __FUNCTION__, file ? file : "(null)", static_cast<long long>(r));
        goto __ret;
    }

    in_size = GetSize();

    buf->Free();
    buf->Alloc(in_size);

    if(LW_RESULT r = Read(buf->GetData(), in_size, &out_size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] Read failed: file={}, in_size={}, ret={}",
                     __FUNCTION__, file ? file : "(null)", in_size, static_cast<long long>(r));
        goto __ret;
    }
    if(in_size != out_size)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFile::SaveFileBuffer(const char* file, lwIBuffer* buf)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD in_size;
    DWORD out_size;
    DWORD access_flag;
    DWORD creation_flag;
    DWORD attributes_flag;

    access_flag = GENERIC_WRITE;
    creation_flag = CREATE_ALWAYS;
    attributes_flag = FILE_FLAG_SEQUENTIAL_SCAN;

    if(file == 0 || buf == 0)
        goto __ret;

    if(buf->GetSize() == 0)
        goto __ret;

    if(LW_RESULT r = CreateFile(file, access_flag, FILE_SHARE_READ, 0, creation_flag, attributes_flag); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] CreateFile failed: file={}, ret={}",
                     __FUNCTION__, file ? file : "(null)", static_cast<long long>(r));
        goto __ret;
    }

    in_size = buf->GetSize();

    if(LW_RESULT r = Write(buf->GetData(), in_size, &out_size); LW_FAILED(r))
    {
        ToLogService("errors", LogLevel::Error,
                     "[{}] Write failed: file={}, in_size={}, ret={}",
                     __FUNCTION__, file ? file : "(null)", in_size, static_cast<long long>(r));
        goto __ret;
    }
    if(in_size != out_size)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwFile::Close()
{
    LW_RESULT ret = LW_RET_FAILED;

    if(_handle == INVALID_HANDLE_VALUE)
        goto __addr_ret_ok;

    if(CloseHandle(_handle) == 0)
        goto __ret;

    _handle = INVALID_HANDLE_VALUE;

__addr_ret_ok:
    ret = LW_RET_OK;
__ret:
    return ret;
}


LW_RESULT lwFile::Read(void* buf, DWORD in_size, DWORD* out_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(out_size == 0)
        out_size = &__dummy;

    if(ReadFile(_handle, buf, in_size, out_size, 0) == 0)
        goto __ret;
    if(in_size != *out_size)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwFile::Write(const void* buf, DWORD in_size, DWORD* out_size)
{
    LW_RESULT ret = LW_RET_FAILED;

    if(out_size == 0)
        out_size = &__dummy;

    if(WriteFile(_handle, buf, in_size, out_size, 0) == 0)
        goto __ret;
    if(in_size != *out_size)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}
LW_RESULT lwFile::Seek(long offset, DWORD flag) 
{
    LW_RESULT ret = LW_RET_FAILED;

    if(SetFilePointer(_handle, offset, 0, flag) == INVALID_SET_FILE_POINTER)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwFile::GetCreationTime(SYSTEMTIME* st)
{
    LW_RESULT ret = LW_RET_FAILED;

	FILETIME ft, lft;

	memset(&ft, 0, sizeof(FILETIME));

	if(GetFileTime(_handle, NULL, NULL, &ft) == 0)
        goto __ret;

    memset(&lft, 0, sizeof(lft));

    if(FileTimeToLocalFileTime(&ft, &lft) == 0)
        goto __ret;

	memset(st, 0, sizeof(SYSTEMTIME));

    if(FileTimeToSystemTime(&lft, st) == 0)
        goto __ret;

    ret = LW_RET_OK;
__ret:
    return ret;
}

LW_RESULT lwFile::CheckExisting(const char* path, DWORD check_directory)
{
    LW_RESULT ret = LW_RET_FAILED;

    DWORD attr = GetFileAttributes(path);
    if(attr == INVALID_FILE_ATTRIBUTES)
        goto __ret;

    if(check_directory && ((attr &FILE_ATTRIBUTE_DIRECTORY) == 0))
        goto __ret;
    
    ret = LW_RET_OK;
__ret:
    return ret;

}

LW_RESULT lwFile::MoveData(DWORD src_pos, DWORD dst_pos, DWORD size)
{
    LW_RESULT ret = LW_RET_OK;

    if(src_pos == dst_pos)
        goto __addr_ret_ok;

	{
    DWORD src_pos_end = src_pos + size;
    DWORD dst_pos_end = dst_pos + size;

    const DWORD max_move_chunk_size = 1024 * 1024; // 1M bytes every moving

    DWORD op_size;
    DWORD move_chunk_size;
    DWORD s;

    op_size = 0;
    move_chunk_size = max_move_chunk_size;

        BYTE* move_buf = new(BYTE[max_move_chunk_size]);

        int loop_flag = 1;

        if ((dst_pos > src_pos && dst_pos < src_pos_end)
            || (dst_pos_end > src_pos && dst_pos_end < src_pos_end))
        {
            // overlapped region
            while (loop_flag)
            {
                if (src_pos + op_size + move_chunk_size > src_pos_end)
                {
                    move_chunk_size = src_pos_end - src_pos - op_size;
                    loop_flag = 0;
                }

                //s = move_chunk_size;

                // read src data
                Seek(src_pos_end - op_size - move_chunk_size, FILE_BEGIN);
                Read(move_buf, move_chunk_size, &s);
                assert(s == move_chunk_size);

                // write dst data
                Seek(dst_pos_end - op_size - move_chunk_size, FILE_BEGIN);
                Write(move_buf, move_chunk_size, &s);
                assert(s == move_chunk_size);
            }
        }
        else
        {
            do
            {
                if (src_pos + op_size + move_chunk_size > src_pos_end)
                {
                    move_chunk_size = src_pos_end - src_pos - op_size;
                    loop_flag = 0;
                }

                //s = move_chunk_size;

                // read src data
                Seek(src_pos + op_size, FILE_BEGIN);
                Read(move_buf, move_chunk_size, &s);
                assert(s == move_chunk_size);

                // write dst data
                Seek(dst_pos + op_size, FILE_BEGIN);
                Write(move_buf, move_chunk_size, &s);
                assert(s == move_chunk_size);


            } while (loop_flag);
        }

        delete[] move_buf;
    }
__addr_ret_ok:
    ret = LW_RET_OK;

    return ret;
}

LW_RESULT lwFile::ReplaceData(DWORD pos, const void* buf, DWORD size)
{
    Seek(pos, FILE_BEGIN);

    DWORD s;
    Write(buf, size, &s);

    return LW_RET_OK;
}

LW_RESULT lwFile::InsertData(DWORD pos, const void* buf, DWORD size)
{
    DWORD cur_size = GetSize();

    if((pos > cur_size) || (size < 0))
        return LW_RET_FAILED;

    Seek(size, FILE_END);
    SetEnd();

    if(pos < cur_size)
    {
        DWORD move_size = cur_size - pos;

        MoveData(pos, cur_size + size - move_size, move_size); 
    }

    DWORD s;
    Seek(pos, FILE_BEGIN);
    Write(buf, size, &s);

    return LW_RET_OK;

}

LW_RESULT lwFile::RemoveData(DWORD pos, DWORD size)
{
    DWORD cur_size = GetSize();

    if(pos >= cur_size)
        return LW_RET_FAILED;

    DWORD end_pos = pos + size;
    DWORD truncate_pos;

    if(end_pos > cur_size)
    {
        truncate_pos = pos;        
    }
    else
    {
        MoveData(end_pos, pos, cur_size - end_pos);        
        truncate_pos = cur_size - size;
    }

    Seek(truncate_pos, FILE_BEGIN);
    SetEnd();

    return LW_RET_OK;
}

// lwFileDialog
LW_STD_IMPLEMENTATION(lwFileDialog)

LW_RESULT lwFileDialog::GetOpenFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title, const char* filter, int flag)
{
	OPENFILENAME	ofn;				// common dialog box structure
    memset(&ofn,0,sizeof(ofn));

	//char ret_buf[1024]={"\0"};
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = num;
	ofn.lpstrFilter = filter;
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = dir;
	ofn.Flags = flag;
    ofn.nFilterIndex = 1;
	// Display the Open dialog box. 
    if(::GetOpenFileName(&ofn) != TRUE) 
		return LW_RET_FAILED;

    // MultiSelected style , break file and directory
    if( flag & OFN_FILEMUSTEXIST ) 
    {
        *(ofn.lpstrFile + ofn.nFileOffset - 1 ) = '\0';
    }

	//strcpy(buf,ofn.lpstrFile);
    return LW_RET_OK;
}

LW_RESULT lwFileDialog::GetSaveFileName(HWND hwnd, char* buf, int num, const char* dir, const char* title, const char* filter, const char* ext, int flag)
{
    OPENFILENAME	ofn;				// common dialog box structure
    memset(&ofn,0,sizeof(OPENFILENAME));

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = buf;
    ofn.nMaxFile = num;
    ofn.lpstrFilter = filter;
    ofn.lpstrTitle = title;
    ofn.lpstrInitialDir = dir;
    ofn.lpstrDefExt = ext;
    ofn.Flags = flag;
    ofn.nFilterIndex = 1;
    // Display the Open dialog box. 
    
    return (::GetSaveFileName(&ofn) == TRUE) ? LW_RET_OK : LW_RET_FAILED;
}


LW_END

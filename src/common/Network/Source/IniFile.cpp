
#include "IniFile.h"
_DBC_USING

char * ltrim(char * src){
	if(!src)return 0;
	char *l_ret =src;
	while(*l_ret){
		if((*l_ret ==' ')||(*l_ret ==0x9)||(*l_ret ==0xA)||(*l_ret ==0xD))
			l_ret++;
		else
			break;
	}
	return l_ret;
}
char * rtrim(char *src){
	if(!src)return 0;
	char *l_tmp =src+strlen(src)-1;
	while(l_tmp >=src){
		if((*l_tmp ==' ')||(*l_tmp ==0x9)||(*l_tmp ==0xA)||(*l_tmp ==0xD))
			l_tmp--;
		else{
			break;
		}
	}
	*(l_tmp+1)	=0;
	return src;
}
char	*trim(char *src){
	return rtrim(ltrim(src));
}
IniFile::IniFile(const char *filename)
:m_rw(false),m_update(false)
,m_sectcount(0),m_sectsize(0),m_sect(0)
{
	m_filename[0]	=m_filename[1]	=0;
	if(filename && (strlen(filename) >0)){
		ReadFile(filename);
	}
}
IniFile::~IniFile(){
   if(m_rw) Flush();
	for(int i=0;i<m_sectcount;i++){
		delete m_sect[i];
	}
	delete []m_sect;
};
void IniFile::Flush(){
	if(!m_update)return;
	m_update	=false;

}
void IniFile::ReadFile(const char *filename){
	m_rw	=false;
	uLong	l_len	=uLong(strlen(filename));
	if((l_len	>0) &&(l_len <512)){
		strcpy(m_filename,filename);
	}else{
		THROW_EXCP(excpFile,"!");
	}
	FILE * l_file	=fopen(filename,m_rw?"r+t":"rt");
	if(!l_file &&m_rw){
		l_file	=fopen(filename,"w+t");
		if(l_file)	m_update	=true;
	}
	if(!l_file){
		THROW_EXCP(excpFile, ":" + filename + " !");
	}
	try{
		char	l_szbuf[1024];
		int	l_lino	=0;
		IniSection *l_sect	=0;
		while( !feof( l_file ) )
		{
			char	*l_str1 = fgets(l_szbuf,1024,l_file);
			if( !l_str1 && ferror( l_file )){
				THROW_EXCP(excpFile,"");
			}
			l_lino	++;
			if(!l_str1||!strlen(trim(l_szbuf))||l_szbuf[0]==';'||l_szbuf[0]=='#'||l_szbuf[0]=='/'){
				continue;
			}
			if((l_str1	=ltrim(l_szbuf))[0]=='['){
				char	*l_str2	=strchr(l_str1+1,']');
				if(!l_str2){
					THROW_EXCP(excpFile, "INI:" + std::to_string(l_lino) + "");
				}
				*l_str2	=0;
				l_sect	=AddSection(trim(l_str1+1));
			}else	if(l_str1	=strchr(l_szbuf,'=')){
				*l_str1	=0;
				char	*l_str2;
				if(!l_sect||!*(l_str2	=trim(l_szbuf))){
					THROW_EXCP(excpFile, "INI:" + std::to_string(l_lino) + "");
				}
				char	*l_str3	=strstr(l_str1+1,"//");
				if(l_str3){*l_str3	=0;}
				l_sect->AddItem(l_str2,trim(l_str1+1));
			}else{
				THROW_EXCP(excpFile, "INI:" + std::to_string(l_lino) + "");
			}
		}
	}catch(...){
		fclose(l_file);
		throw;
	}
	fclose( l_file );
}
IniSection & IniFile::operator[](int i){
	IniSection	*l_is	=0;
	if(i>=0 && i<m_sectcount){
		l_is	=m_sect[i];
	}else if(i==m_sectcount){
		l_is	=new IniSection;
		if(m_sectcount >=m_sectsize){
			IniSection	**l_sect	=new IniSection*[m_sectsize +16];
			MemCpy((char*)l_sect,(char*)m_sect,sizeof(IniSection*)*m_sectcount);
			MemSet((char*)(l_sect+m_sectcount),0,sizeof(IniSection*)*(m_sectsize+16-m_sectcount));
			delete []m_sect;
			m_sectsize	+=16;
			m_sect	=l_sect;
		}
		m_sect[m_sectcount]	=l_is;
		m_sectcount	++;
	}else{
		THROW_EXCP(excpArr,"IniFile:" + std::to_string(i) + "");
	}
	return *l_is;
}

IniSection &IniFile::operator[](const char *sectname)const{
	int i = 0; 
	for(;i<m_sectcount;i++){
		if(m_sect[i]->m_sectname	==sectname){
			break;
		}
	}
	if(i<m_sectcount){
		return *(m_sect[i]);
	}else{
		THROW_EXCP(excpIniF,"Section:" + sectname + " .");
	}
}
IniSection * IniFile::AddSection(const char *sectname){
	IniSection *l_sect	=&((*this)[m_sectcount]);
	l_sect->m_sectname	=sectname;
	return l_sect;
}
IniSection::~IniSection(){
	for(int i=0;i<m_itemcount;i++){
		delete m_item[i];
	}
	delete []m_item;
}
IniItem & IniSection::operator[](int i){
	IniItem	*l_it	=0;
	if(i>=0 && i<m_itemcount)
		l_it	=m_item[i];
	else if(i==m_itemcount){
		l_it	=new IniItem;
		if(m_itemcount >=m_itemsize){
			IniItem	**l_item	=new IniItem*[m_itemsize +16];
			MemCpy((char*)l_item,(char*)m_item,sizeof(IniItem*)*m_itemcount);
			MemSet((char*)(l_item+m_itemcount),0,sizeof(IniItem*)*(m_itemsize+16-m_itemcount));
			delete []m_item;
			m_itemsize	+=16;
			m_item	=l_item;
		}
		m_item[m_itemcount]	=l_it;
		m_itemcount	++;
	}else{
		THROW_EXCP(excpArr,std::string("Section:") + m_sectname + " <<i<<");
	}
	return *l_it;
}
std::string &IniSection::operator[](const char *name)const{
	int i = 0;
	for(;i<m_itemcount;i++){
		if(m_item[i]->name	==name){
			break;
		}
	}
	if(i<m_itemcount){
		return m_item[i]->value;
	}else{
		THROW_EXCP(excpIniF, std::string("Section:") + m_sectname + " Name:" + name + " .");
	}
}
IniItem * IniSection::AddItem(const char *name,const char *value){
	IniItem *l_item	=&((*this)[m_itemcount]);
	l_item->name	=name;
	l_item->value	=value;
	return l_item;
}

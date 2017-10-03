//------------------------------------------------------------------------------
// emDirEntry.cpp
//
// Copyright (C) 2005-2012,2016 Oliver Hamann.
//
// Homepage: http://eaglemode.sourceforge.net/
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 3 as published by the
// Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License version 3 for
// more details.
//
// You should have received a copy of the GNU General Public License version 3
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#if defined(_WIN32)
#	include <windows.h>
#	include <aclapi.h>
#else
#	include <pwd.h>
#	include <grp.h>
#	include <unistd.h>
#endif
#include <sys/types.h>
#include <emFileMan/emDirModel.h>


emDirEntry::emDirEntry()
{
	Data=&EmptyData;
}


emDirEntry::emDirEntry(const emString & path)
{
	Data=&EmptyData;
	Load(path);
}


emDirEntry::emDirEntry(const emString & parentPath, const emString & name)
{
	Data=&EmptyData;
	Load(parentPath,name);
}


emDirEntry::emDirEntry(const emDirEntry & dirEntry)
{
	Data=dirEntry.Data;
	Data->RefCount++;
}


emDirEntry::~emDirEntry()
{
	if (!--Data->RefCount) FreeData();
}


emDirEntry & emDirEntry::operator = (const emDirEntry & dirEntry)
{
	dirEntry.Data->RefCount++;
	if (!--Data->RefCount) FreeData();
	Data=dirEntry.Data;
	return *this;
}


bool emDirEntry::operator == (const emDirEntry & dirEntry) const
{
	if (Data!=dirEntry.Data) {
		if (
			Data->StatErrNo!=dirEntry.Data->StatErrNo ||
			Data->LStatErrNo!=dirEntry.Data->LStatErrNo ||
			Data->TargetPathErrNo!=dirEntry.Data->TargetPathErrNo ||
			Data->Path!=dirEntry.Data->Path ||
			Data->Name!=dirEntry.Data->Name ||
			Data->TargetPath!=dirEntry.Data->TargetPath ||
			Data->Owner!=dirEntry.Data->Owner ||
			Data->Group!=dirEntry.Data->Group ||
			Data->Hidden!=dirEntry.Data->Hidden ||
			memcmp(&Data->Stat,&dirEntry.Data->Stat,sizeof(Data->Stat))!=0 ||
			memcmp(&Data->LStat,&dirEntry.Data->LStat,sizeof(Data->LStat))!=0
#if defined(_WIN32)
			|| Data->WndsFileAttributes!=dirEntry.Data->WndsFileAttributes ||
			Data->Drive!=dirEntry.Data->Drive ||
			Data->DriveType!=dirEntry.Data->DriveType ||
			Data->DriveName!=dirEntry.Data->DriveName ||
			Data->DriveFileSystem!=dirEntry.Data->DriveFileSystem
#endif
		) return false;
	}
	return true;
}


void emDirEntry::Load(const emString & path)
{
	PrivLoad(path,emGetNameInPath(path));
}


void emDirEntry::Load(const emString & parentPath, const emString & name)
{
	PrivLoad(emGetChildPath(parentPath,name),name);
}


void emDirEntry::Clear()
{
	if (!--Data->RefCount) FreeData();
	Data=&EmptyData;
}


unsigned int emDirEntry::GetDataRefCount() const
{
	return Data==&EmptyData ? UINT_MAX/2 : Data->RefCount;
}


void emDirEntry::PrivLoad(const emString & path, const emString & name)
{
#if defined(_WIN32)
	WIN32_FILE_ATTRIBUTE_DATA fad;
	PSECURITY_DESCRIPTOR pSd;
	PSID pOwnerSid, pGroupSid;
	SID_NAME_USE snu;
	CHAR str[MAX_PATH+1],str2[MAX_PATH+1];
	DWORD sz,sz2;
	BOOL b;

	if (!--Data->RefCount) FreeData();
	Data=new SharedData;
	Data->Path=path;
	Data->Name=name;
	Data->TargetPath=Data->Path;
	if (em_stat(Data->Path,&Data->Stat)) {
		Data->LStatErrNo=errno;
		Data->StatErrNo=errno;
		memset(&Data->Stat,0,sizeof(struct em_stat));
	}
	Data->Owner="";
	Data->Group="";
	if (
		GetNamedSecurityInfo(
			(char*)Data->Path.Get(),SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION,
			&pOwnerSid,&pGroupSid,NULL,NULL,&pSd
		)==ERROR_SUCCESS
	) {
		sz=sizeof(str);
		sz2=sizeof(str2);
		if (LookupAccountSid(NULL,pOwnerSid,str,&sz,str2,&sz2,&snu)) {
			Data->Owner=str;
		}
		sz=sizeof(str);
		sz2=sizeof(str2);
		if (LookupAccountSid(NULL,pGroupSid,str,&sz,str2,&sz2,&snu)) {
			Data->Group=str;
		}
		LocalFree(pSd);
	}
	b=GetFileAttributesEx(Data->Path.Get(),GetFileExInfoStandard,&fad);
	Data->WndsFileAttributes = b ? fad.dwFileAttributes : 0;
	if (Data->Path.GetLen()==3 && Data->Path[1]==':' && Data->Path[2]=='\\') {
		Data->Drive=true;
	}
	Data->Hidden=
		!Data->Drive &&
		(Data->WndsFileAttributes&FILE_ATTRIBUTE_HIDDEN)!=0
	;
	if (Data->Drive) {
		Data->DriveType=::GetDriveType(Data->Path.Get());
		if (
			::GetVolumeInformation(
				Data->Path.Get(),str,sizeof(str),NULL,
				NULL,NULL,str2,sizeof(str2)
			)
		) {
			Data->DriveName=str;
			Data->DriveFileSystem=str2;
		}
	}
#else
	char tmp[1024];
#if !defined(ANDROID)
	struct passwd pwbuf;
	struct group grbuf;
#endif
	struct passwd * pw;
	struct group * gr;
	int i;

	if (!--Data->RefCount) FreeData();
	Data=new SharedData;
	Data->Path=path;
	Data->Name=name;
	Data->TargetPath=Data->Path;
	if (em_lstat(Data->Path,&Data->Stat)) {
		Data->LStatErrNo=errno;
		if (em_stat(Data->Path,&Data->Stat)) {
			Data->StatErrNo=errno;
			memset(&Data->Stat,0,sizeof(struct em_stat));
		}
		else {
			Data->LStat=(struct em_stat*)malloc(sizeof(struct em_stat));
			memset(Data->LStat,0,sizeof(struct em_stat));
		}
	}
	else if (S_ISLNK(Data->Stat.st_mode)) {
		Data->LStat=(struct em_stat*)malloc(sizeof(struct em_stat));
		memcpy(Data->LStat,&Data->Stat,sizeof(struct em_stat));
		if (em_stat(Data->Path,&Data->Stat)) {
			Data->StatErrNo=errno;
			memset(&Data->Stat,0,sizeof(struct em_stat));
		}
		i=readlink(Data->Path,tmp,sizeof(tmp)-1);
		if (i<0) {
			Data->TargetPathErrNo=errno;
			tmp[0]=0;
		}
		else {
			tmp[i]=0;
		}
		Data->TargetPath=tmp;
	}

#if defined(ANDROID)
	pw=getpwuid(Data->Stat.st_uid);
	i=0;
#else
	i=getpwuid_r(Data->Stat.st_uid,&pwbuf,tmp,sizeof(tmp),&pw);
#endif
	if (i==0 && pw && pw->pw_name) Data->Owner=pw->pw_name;
	else Data->Owner=emString::Format("%lu",(unsigned long)Data->Stat.st_uid);

#if defined(ANDROID)
	gr=getgrgid(Data->Stat.st_gid);
	i=0;
#else
	i=getgrgid_r(Data->Stat.st_gid,&grbuf,tmp,sizeof(tmp),&gr);
#endif
	if (i==0 && gr && gr->gr_name) Data->Group=gr->gr_name;
	else Data->Group=emString::Format("%lu",(unsigned long)Data->Stat.st_gid);

	Data->Hidden=(Data->Name[0]=='.');
#endif
}


void emDirEntry::FreeData()
{
	EmptyData.RefCount=UINT_MAX/2;
	if (Data!=&EmptyData) delete Data;
}


emDirEntry::SharedData::SharedData()
{
	RefCount=1;
	StatErrNo=0;
	LStatErrNo=0;
	TargetPathErrNo=0;
	Hidden=false;
	memset(&Stat,0,sizeof(struct em_stat));
	LStat=&Stat;
#if defined(_WIN32)
	WndsFileAttributes=0;
	Drive=false;
	DriveType=0;
#endif
}


emDirEntry::SharedData::~SharedData()
{
	if (LStat!=&Stat) free(LStat);
}


emDirEntry::SharedData emDirEntry::EmptyData;

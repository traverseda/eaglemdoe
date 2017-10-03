//------------------------------------------------------------------------------
// emRgbImageFileModel.h
//
// Copyright (C) 2004-2008,2014 Oliver Hamann.
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

#ifndef emRgbImageFileModel_h
#define emRgbImageFileModel_h

#ifndef emImageFile_h
#include <emCore/emImageFile.h>
#endif


class emRgbImageFileModel : public emImageFileModel {

public:

	// RGB = SGI Image File Format

	static emRef<emRgbImageFileModel> Acquire(
		emContext & context, const emString & name, bool common=true
	);

protected:

	emRgbImageFileModel(emContext & context, const emString & name);
	virtual ~emRgbImageFileModel();
	virtual void TryStartLoading() throw(emException);
	virtual bool TryContinueLoading() throw(emException);
	virtual void QuitLoading();
	virtual void TryStartSaving() throw(emException);
	virtual bool TryContinueSaving() throw(emException);
	virtual void QuitSaving();
	virtual emUInt64 CalcMemoryNeed();
	virtual double CalcFileProgress();

private:

	int Read8();
	int Read16();
	int Read32();

	struct LoadingState {
		int Storage,BytesPerChannel,XSize,YSize,ZSize;
		int PixMin,PixMax,NextY,NextZ,ZUse;
		bool ImagePrepared;
		FILE * File;
		emUInt32 * OffsetTable;
	};

	LoadingState * L;
};


#endif

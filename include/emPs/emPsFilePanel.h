//------------------------------------------------------------------------------
// emPsFilePanel.h
//
// Copyright (C) 2006-2008,2016 Oliver Hamann.
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

#ifndef emPsFilePanel_h
#define emPsFilePanel_h

#ifndef emFilePanel_h
#include <emCore/emFilePanel.h>
#endif

#ifndef emPsFileModel_h
#include <emPs/emPsFileModel.h>
#endif

#ifndef emPsDocumentPanel_h
#include <emPs/emPsDocumentPanel.h>
#endif


class emPsFilePanel : public emFilePanel {

public:

	emPsFilePanel(ParentArg parent, const emString & name,
	              emPsFileModel * fileModel=NULL,
	              bool updateFileModel=true);

	virtual ~emPsFilePanel();

	virtual void SetFileModel(emFileModel * fileModel,
	                          bool updateFileModel=true);

	virtual emString GetIconFileName() const;

protected:

	virtual bool Cycle();
	virtual bool IsOpaque() const;
	virtual void Paint(const emPainter & painter, emColor canvasColor) const;
	virtual void LayoutChildren();

private:

	void UpdateDocPanel();
	void HaveDocPanel(bool haveIt);

	emPsDocumentPanel * DocPanel;
};


#endif

#ifndef SFTWX_MAINFRAME_H
#define SFTWX_MAINFRAME_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dir.h>
#include <wx/filename.h>

#include "CountingDirTraverser.hpp"


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    wxBoxSizer* mainSizer;
    wxBoxSizer* progressAndStartSizer;

    wxMenuBar* menuBar;
    wxMenu* mainMenu;

    wxButton* btnSrc;
    wxButton* btnDst;
    wxCheckBox* mp3OnlyCheckBox;
    wxGauge* progressIndicator;
    wxButton* btnStart;

    wxString m_src_dir;
    wxString m_dst_dir;
    bool m_mp3Only = false;

    void OnChooseSrc(wxCommandEvent& event);
    void OnChooseDst(wxCommandEvent& event);
    void onMp3OnlyToggled(wxCommandEvent& event);
    void OnStartCopy(wxCommandEvent& event);

    void OnExit(wxCommandEvent& event);

    void PerformCopy(wxDir& src, wxDir& dst);
    void CopyDir(wxDir& src, wxDir& dst, int& filesCopied);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_MAIN_SRC = wxID_HIGHEST + 1,
    ID_MAIN_DST = wxID_HIGHEST + 2,
    ID_MAIN_START = wxID_HIGHEST + 3,
    ID_MP3_ONLY = wxID_HIGHEST + 4,
    ID_PROGRESS = wxID_HIGHEST + 5
};

#endif //SFTWX_MAINFRAME_H
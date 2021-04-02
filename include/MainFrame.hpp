#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dir.h>
#include <wx/filename.h>

#include "CountingDirTraverser.hpp"
#include "SubdirListTraverser.hpp"


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    wxBoxSizer* main_sizer;
    wxBoxSizer* progress_and_start_sizer;

    wxMenuBar* menu_bar;
    wxMenu* menu_main;

    wxButton* button_src;
    wxButton* button_dst;
    wxCheckBox* toggle_mp3_only;
    wxGauge* progressbar;
    wxButton* button_start;

    wxString m_src_dir;
    wxString m_dst_dir;
    bool m_mp3_only = false;

    void OnChooseSrc(wxCommandEvent& event);
    void OnChooseDst(wxCommandEvent& event);
    void onMp3OnlyToggled(wxCommandEvent& event);
    void OnStartCopy(wxCommandEvent& event);

    void OnExit(wxCommandEvent& event);

    void PerformCopy(const wxDir& src, const wxDir& dst);
    void CopyDir(const wxDir& src, const wxDir& dst, int& files_copied);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_MAIN_SRC = wxID_HIGHEST + 1,
    ID_MAIN_DST = wxID_HIGHEST + 2,
    ID_MAIN_START = wxID_HIGHEST + 3,
    ID_MP3_ONLY = wxID_HIGHEST + 4,
    ID_PROGRESS = wxID_HIGHEST + 5
};

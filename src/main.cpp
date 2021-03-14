#include "MainFrame.hpp"


class App : public wxApp {
public:
    bool OnInit();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_BUTTON(ID_MAIN_SRC, MainFrame::OnChooseSrc)
                EVT_BUTTON(ID_MAIN_DST, MainFrame::OnChooseDst)
                EVT_CHECKBOX(ID_MP3_ONLY, MainFrame::onMp3OnlyToggled)
                EVT_BUTTON(ID_MAIN_START, MainFrame::OnStartCopy)
                EVT_MENU(wxID_EXIT, MainFrame::OnExit)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(App);


bool App::OnInit() {
    MainFrame* frame = new MainFrame("SFTwx", wxDefaultPosition, wxSize(600, 400));
    frame->Show(true);
    return true;
}
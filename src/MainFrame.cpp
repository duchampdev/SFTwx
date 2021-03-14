#include "MainFrame.hpp"


MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size): wxFrame(nullptr, wxID_ANY, title, pos, size) {
    mainMenu = new wxMenu();
    mainMenu->Append(wxID_EXIT);

    menuBar = new wxMenuBar();
    menuBar->Append(mainMenu, "Menu");

    this->SetMenuBar(menuBar);
    this->CreateStatusBar();
    this->SetStatusText("");

    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->SetMinSize(600, 300);
    progressAndStartSizer = new wxBoxSizer(wxHORIZONTAL);
    progressAndStartSizer->SetMinSize(600, 0);

    btnSrc = new wxButton(this, ID_MAIN_SRC, "Quellverzeichnis");
    btnDst = new wxButton(this, ID_MAIN_DST, "Zielverzeichnis", wxDefaultPosition, wxDefaultSize);
    mp3OnlyCheckBox = new wxCheckBox(this, ID_MP3_ONLY, "nur MP3-Dateien", wxDefaultPosition, wxDefaultSize);
    progressIndicator = new wxGauge(this, ID_PROGRESS, 100, wxDefaultPosition, wxDefaultSize);
    btnStart = new wxButton(this, ID_MAIN_START, "Kopieren starten");


    progressAndStartSizer->AddSpacer(10);
    progressAndStartSizer->Add(progressIndicator, 1, wxALL, 20);
    progressAndStartSizer->AddSpacer(10);
    progressAndStartSizer->Add(btnStart, 1, wxALL, 20);
    progressAndStartSizer->AddSpacer(10);

    mainSizer->Add(btnSrc, 0, wxEXPAND, 20);
    mainSizer->AddSpacer(10);
    mainSizer->Add(btnDst, 0, wxEXPAND, 20);
    mainSizer->AddSpacer(10);
    mainSizer->Add(mp3OnlyCheckBox, 0, wxALL, 20);
    mainSizer->AddSpacer(10);
    mainSizer->Add(progressAndStartSizer, 0, wxEXPAND, 20);

    SetSizerAndFit(mainSizer);
}

void MainFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MainFrame::OnChooseSrc(wxCommandEvent &event) {
    wxDirDialog dialog(this, "Quellverzeichnis waehlen");
    if(dialog.ShowModal() == wxID_OK) {
        m_src_dir = dialog.GetPath();
        btnSrc->SetLabelText(m_src_dir);
    } else {
        this->SetStatusText("Kein neues Quellverzeichnis ausgewaehlt");
    }
}

void MainFrame::OnChooseDst(wxCommandEvent &event) {
    wxDirDialog dialog(this, "Zielverzeichnis waehlen");
    if(dialog.ShowModal() == wxID_OK) {
        m_dst_dir = dialog.GetPath();
        btnDst->SetLabelText(m_dst_dir);
    } else {
        this->SetStatusText("Kein neues Zielverzeichnis ausgewaehlt");
    }
}

void MainFrame::onMp3OnlyToggled(wxCommandEvent &event) {
    m_mp3Only = mp3OnlyCheckBox->GetValue();
}

void MainFrame::OnStartCopy(wxCommandEvent &event) {
    if (!wxDirExists(m_src_dir) || !wxDirExists(m_dst_dir)) {
        this->SetStatusText("Quell- oder nicht Zielverzeichnis vorhanden.");
        return;
    }
    wxDir src_dir(m_src_dir);
    wxDir dst_dir(m_dst_dir);
    if(dst_dir.HasFiles()) {
        this->SetStatusText("Zielverzeichnis beinhaltet Dateien. Abbruch");
        return;
    }
    wxString srcLastDirName = src_dir.GetName().AfterLast('/').AfterLast('\\');
    std::cout << "checking for " << dst_dir.GetNameWithSep() + srcLastDirName << std::endl;
    if(wxDirExists(dst_dir.GetNameWithSep() + srcLastDirName)) {
        this->SetStatusText("Zielverzeichnis beinhaltet Quellverzeichnis. Abbruch");
        return;
    }
    this->SetStatusText("Starte Kopieren...");
    this->PerformCopy(src_dir, dst_dir);
}

void MainFrame::PerformCopy(wxDir& src, wxDir& dst) {
    int num_files_total = 0;
    CountingDirTraverser traverser(num_files_total);
    src.Traverse(traverser);
    std::cout << "found " << num_files_total << " files" << std::endl;
}

void MainFrame::CopyDir(wxDir &src, wxDir &dst) {}
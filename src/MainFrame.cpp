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
    if(wxDir(m_dst_dir).HasFiles()) {
        this->SetStatusText("Zielverzeichnis beinhaltet Dateien. Abbruch");
        return;
    }
    wxString srcLastDirName = wxFileName(m_src_dir).GetName();
    std::cout << "checking for " << dst_dir.GetNameWithSep() + srcLastDirName << std::endl;
    if(wxDirExists(dst_dir.GetNameWithSep() + srcLastDirName)) {
        this->SetStatusText("Zielverzeichnis beinhaltet Quellverzeichnis. Abbruch");
        return;
    }
    this->SetStatusText("Starte Kopieren...");
    this->PerformCopy(src_dir, dst_dir);
}

void MainFrame::PerformCopy(wxDir& src, wxDir& dst) {
    int numFilesTotal = 0;
    int filesCopied = 0;
    CountingDirTraverser traverser(numFilesTotal, m_mp3Only);
    wxDir(src.GetName()).Traverse(traverser);
    std::cout << "found " << numFilesTotal << " files" << std::endl;
    progressIndicator->SetRange(numFilesTotal);
    CopyDir(src, dst, filesCopied);
    std::cout << "'copied' " << filesCopied << " files." << std::endl;
}

void MainFrame::CopyDir(wxDir& src, wxDir& dst, int& filesCopied) {
    // copy files in DFS-manner
    wxArrayString subdirs;
    SubdirListTraverser traverser(subdirs);

    // create src dir in dst
    const wxString dst_targetsubdir_path(dst.GetNameWithSep() + wxFileName(src.GetName()).GetName());
    if(!wxDir::Make(dst_targetsubdir_path)) {
        this->SetStatusText("Fehler beim Kopieren! Ziel-Subverzeichnus konnte nicht erstellt werden. Verzeichnis ausgelassen");
        std::cout << "could not create dir: " << dst_targetsubdir_path << std::endl;
        return;
    }
    wxDir dst_targetsubdir(dst_targetsubdir_path);

    src.Traverse(traverser);
    subdirs.Sort();
    for(auto& subdir : subdirs) {
        // recursively call CopyDir for contained dirs
        wxDir newSrc(subdir);
        CopyDir(newSrc, dst_targetsubdir, filesCopied);
    }

    wxArrayString files;
    wxDir::GetAllFiles(src.GetName(), &files, wxEmptyString, wxDIR_FILES);
    for(auto& file: files) {
        if(m_mp3Only && !file.Lower().EndsWith(".mp3")) continue;
        if(wxCopyFile(file, dst_targetsubdir.GetNameWithSep() + wxFileName(file).GetFullName(), false)) {
            progressIndicator->SetValue(++filesCopied);
        } else {
            this->SetStatusText("Fehler beim Kopieren einer Datei. Datei ausgelassen");
            std::cout << "failed to copy file " << file << std::endl;
        }
    }

}
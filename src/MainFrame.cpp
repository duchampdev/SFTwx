#include "MainFrame.hpp"


MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size): wxFrame(nullptr, wxID_ANY, title, pos, size) {
    menu_main = new wxMenu();
    menu_main->Append(wxID_EXIT);

    menu_bar = new wxMenuBar();
    menu_bar->Append(menu_main, "Menu");

    this->SetMenuBar(menu_bar);
    this->CreateStatusBar();
    this->SetStatusText("");

    main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->SetMinSize(600, 300);
    progress_and_start_sizer = new wxBoxSizer(wxHORIZONTAL);
    progress_and_start_sizer->SetMinSize(600, 0);

    button_src = new wxButton(this, ID_MAIN_SRC, "Quellverzeichnis");
    button_dst = new wxButton(this, ID_MAIN_DST, "Zielverzeichnis", wxDefaultPosition, wxDefaultSize);
    toggle_mp3_only = new wxCheckBox(this, ID_MP3_ONLY, "nur MP3-Dateien", wxDefaultPosition, wxDefaultSize);
    progressbar = new wxGauge(this, ID_PROGRESS, 100, wxDefaultPosition, wxDefaultSize);
    button_start = new wxButton(this, ID_MAIN_START, "Kopieren starten");


    progress_and_start_sizer->AddSpacer(10);
    progress_and_start_sizer->Add(progressbar, 1, wxALL, 20);
    progress_and_start_sizer->AddSpacer(10);
    progress_and_start_sizer->Add(button_start, 1, wxALL, 20);
    progress_and_start_sizer->AddSpacer(10);

    main_sizer->Add(button_src, 0, wxEXPAND, 20);
    main_sizer->AddSpacer(10);
    main_sizer->Add(button_dst, 0, wxEXPAND, 20);
    main_sizer->AddSpacer(10);
    main_sizer->Add(toggle_mp3_only, 0, wxALL, 20);
    main_sizer->AddSpacer(10);
    main_sizer->Add(progress_and_start_sizer, 0, wxEXPAND, 20);

    SetSizerAndFit(main_sizer);
}

void MainFrame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void MainFrame::OnChooseSrc(wxCommandEvent &event) {
    wxDirDialog dialog(this, "Quellverzeichnis waehlen");
    if(dialog.ShowModal() == wxID_OK) {
        m_src_dir = dialog.GetPath();
        button_src->SetLabelText(m_src_dir);
    } else {
        this->SetStatusText("Kein neues Quellverzeichnis ausgewaehlt");
    }
}

void MainFrame::OnChooseDst(wxCommandEvent &event) {
    wxDirDialog dialog(this, "Zielverzeichnis waehlen");
    if(dialog.ShowModal() == wxID_OK) {
        m_dst_dir = dialog.GetPath();
        button_dst->SetLabelText(m_dst_dir);
    } else {
        this->SetStatusText("Kein neues Zielverzeichnis ausgewaehlt");
    }
}

void MainFrame::onMp3OnlyToggled(wxCommandEvent &event) {
    m_mp3_only = toggle_mp3_only->GetValue();
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
    wxString src_most_inner_dir_name = wxFileName(m_src_dir).GetName();
    std::cout << "checking for " << dst_dir.GetNameWithSep() + src_most_inner_dir_name << std::endl;
    if(wxDirExists(dst_dir.GetNameWithSep() + src_most_inner_dir_name)) {
        this->SetStatusText("Zielverzeichnis beinhaltet Quellverzeichnis. Abbruch");
        return;
    }
    this->SetStatusText("Starte Kopieren...");
    this->PerformCopy(src_dir, dst_dir);
}

void MainFrame::PerformCopy(wxDir& src, wxDir& dst) {
    int num_files_total = 0;
    int files_copied = 0;
    CountingDirTraverser traverser(num_files_total, m_mp3_only);
    wxDir(src.GetName()).Traverse(traverser);
    std::cout << "found " << num_files_total << " files" << std::endl;
    progressbar->SetRange(num_files_total);
    CopyDir(src, dst, files_copied);
    std::cout << "'copied' " << files_copied << " files." << std::endl;
}

void MainFrame::CopyDir(wxDir& src, wxDir& dst, int& files_copied) {
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
        CopyDir(newSrc, dst_targetsubdir, files_copied);
    }

    wxArrayString files;
    wxDir::GetAllFiles(src.GetName(), &files, wxEmptyString, wxDIR_FILES);
    for(auto& file: files) {
        if(m_mp3_only && !file.Lower().EndsWith(".mp3")) continue;
        if(wxCopyFile(file, dst_targetsubdir.GetNameWithSep() + wxFileName(file).GetFullName(), false)) {
            progressbar->SetValue(++files_copied);
        } else {
            this->SetStatusText("Fehler beim Kopieren einer Datei. Datei ausgelassen");
            std::cout << "failed to copy file " << file << std::endl;
        }
    }

}
#pragma once

#include <wx/dir.h>

class CountingDirTraverser: public wxDirTraverser {
public:
    CountingDirTraverser(int& num_files, const bool mp3_only): m_num_files(num_files), m_mp3_only(mp3_only) {}

    wxDirTraverseResult OnFile(const wxString &filename) override {
        if(!m_mp3_only || filename.Lower().EndsWith(".mp3")) {
            m_num_files++;
        }
        return wxDIR_CONTINUE;
    }

    wxDirTraverseResult OnDir(const wxString &dirname) override {
        return wxDIR_CONTINUE;
    }

private:
    int& m_num_files;
    const bool m_mp3_only;
};

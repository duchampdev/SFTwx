#pragma once

#include <wx/dir.h>

class CountingDirTraverser: public wxDirTraverser {
public:
    CountingDirTraverser(int& num_files): m_num_files(num_files) {}

    wxDirTraverseResult OnFile(const wxString &filename) override {
        m_num_files++;
        return wxDIR_CONTINUE;
    }

    wxDirTraverseResult OnDir(const wxString &dirname) override {
        return wxDIR_CONTINUE;
    }

private:
    int& m_num_files;
};

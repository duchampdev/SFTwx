#ifndef SFTWX_SUBDIRLISTTRAVERSER_H
#define SFTWX_SUBDIRLISTTRAVERSER_H

class SubdirListTraverser: public wxDirTraverser {
public:
    SubdirListTraverser(wxArrayString& dir_names): m_dir_names(dir_names) {}

    wxDirTraverseResult OnFile(const wxString &filename) override {
        return wxDIR_IGNORE;
    }

    wxDirTraverseResult OnDir(const wxString &dirname) override {
        m_dir_names.push_back(dirname);
        return wxDIR_IGNORE; // do not recurse further
    }

private:
    wxArrayString& m_dir_names;
};

#endif //SFTWX_SUBDIRLISTTRAVERSER_H

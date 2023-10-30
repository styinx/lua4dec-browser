#ifndef LUA4DEC_BROWSER_APP_HPP
#define LUA4DEC_BROWSER_APP_HPP

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include <wx/stc/stc.h>
#include <wx/treectrl.h>

class App : public wxApp
{
private:
    wxFrame*          m_window  = nullptr;
    wxBoxSizer*       m_left    = nullptr;
    wxBoxSizer*       m_columns = nullptr;
    wxTreeCtrl*       m_tree    = nullptr;
    wxPropertyGrid*   m_grid    = nullptr;
    wxStyledTextCtrl* m_editor  = nullptr;

public:
    App() = default;
    explicit App(const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~App() = default;

    virtual bool OnInit() override;
    void         CreateLayout();
    void         OnDropFiles(wxDropFilesEvent& event);
};

#endif  // LUA4DEC_BROWSER_APP_HPP

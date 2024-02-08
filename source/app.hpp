#ifndef LUA4DEC_BROWSER_APP_HPP
#define LUA4DEC_BROWSER_APP_HPP

#include "lua4dec.hpp"

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

    Ast* m_ast = nullptr;

    void         BuildTree(Ast* ast, wxTreeItemId id);
    wxTreeItemId AppendTreeItem(wxTreeItemId, Assignment);
    wxTreeItemId AppendTreeItem(wxTreeItemId, Call);
    wxTreeItemId AppendTreeItem(wxTreeItemId, Condition);
    wxTreeItemId AppendTreeItem(wxTreeItemId, ForLoop);
    wxTreeItemId AppendTreeItem(wxTreeItemId, ForInLoop);
    wxTreeItemId AppendTreeItem(wxTreeItemId, LocalAssignment);
    wxTreeItemId AppendTreeItem(wxTreeItemId, Return);
    wxTreeItemId AppendTreeItem(wxTreeItemId, TailCall);
    wxTreeItemId AppendTreeItem(wxTreeItemId, WhileLoop);
    void         FillEditor(Ast* ast);
    void         AddText(Assignment); /*
     void         AddText(Call);
     void         AddText(Condition);
     void         AddText(ForLoop);
     void         AddText(ForInLoop);
     void         AddText(LocalAssignment);
     void         AddText(Return);
     void         AddText(TailCall);
     void         AddText(WhileLoop);*/

public:
    App() = default;
    explicit App(const wxString& title, const wxPoint& pos, const wxSize& size);
    virtual ~App() = default;

    virtual bool OnInit() override;
    void         CreateLayout();
    void         OnDropFiles(wxDropFilesEvent& event);
};

#endif  // LUA4DEC_BROWSER_APP_HPP

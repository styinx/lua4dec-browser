#include "app.hpp"

#include <wx/dir.h>
#include <wx/statline.h>

App::App(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxApp()
{
}

void App::BuildTree(Ast* ast, wxTreeItemId id)
{
    for(const auto& element : ast->statements)
    {
        std::visit([this, &id](auto&& s) { AppendTreeItem(id, s); }, element);
    }

    if(ast->child)
    {
        wxTreeItemId child = m_tree->AppendItem(id, "child");
        BuildTree(ast->child, child);
    }
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Assignment ass)
{
    auto child = m_tree->AppendItem(parent, "Assignment");
    m_tree->AppendItem(child, ass.left.name);
    m_tree->AppendItem(child, "Expression");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Call call)
{
    auto child = m_tree->AppendItem(parent, "Call");
    m_tree->AppendItem(child, call.name.name);
    for(const auto& arg : call.arguments)
    {
        m_tree->AppendItem(child, "Expression");
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Condition condition)
{
    auto child = m_tree->AppendItem(parent, "Condition");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, ForLoop loop)
{
    auto child = m_tree->AppendItem(parent, "ForLoop");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, ForInLoop loop)
{
    auto child = m_tree->AppendItem(parent, "ForInLoop");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, LocalAssignment ass)
{
    auto child = m_tree->AppendItem(parent, "LocalAssignment");
    m_tree->AppendItem(child, ass.left.name);
    m_tree->AppendItem(child, "Expression");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Return ret)
{
    auto child = m_tree->AppendItem(parent, "Return");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, TailCall call)
{
    auto child = m_tree->AppendItem(parent, "TailCall");
    m_tree->AppendItem(child, call.name.name);
    for(const auto& arg : call.arguments)
    {
        m_tree->AppendItem(child, "Expression");
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, WhileLoop loop)
{
    auto child = m_tree->AppendItem(parent, "WhileLoop");
    return child;
}

void App::FillEditor(Ast* ast)
{
    StringBuffer buffer;
    print_ast(ast, buffer);

    m_editor->AddText(wxString(buffer.str()));

    for(const auto& element : ast->statements)
    {
    }
}

void App::AddText(Assignment ass)
{
    m_editor->AddText(ass.left.name);
}

bool App::OnInit()
{
    CreateLayout();

    m_window->Center();
    m_window->Show();

    return true;
}

void App::CreateLayout()
{
    m_window = new wxFrame(
        nullptr,
        wxID_ANY,
        "lua4dec-browser",
        wxPoint(50, 50),
        wxSize(800, 600),
        wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE);
    m_window->DragAcceptFiles(true);
    m_window->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(App::OnDropFiles), NULL, this);

    m_left    = new wxBoxSizer(wxVERTICAL);
    m_columns = new wxBoxSizer(wxHORIZONTAL);

    m_tree = new wxTreeCtrl(m_window, wxID_ANY);
    // m_tree->SetWindowStyle(m_tree->GetWindowStyle() | wxBORDER_NONE);

    m_grid = new wxPropertyGrid(m_window, wxID_ANY);
    // m_grid->SetWindowStyle(m_grid->GetWindowStyle() | wxBORDER_NONE);

    wxArrayString choices;
    choices.Add("INT");
    choices.Add("FLOAT");
    m_grid->Append(new wxEnumProperty("Type", wxPG_LABEL, choices));

    m_editor = new wxStyledTextCtrl(m_window, wxID_ANY);
    // m_editor->SetWindowStyle(m_editor->GetWindowStyle() | wxBORDER_NONE);
    m_editor->SetCaretStyle(wxSTC_CARETSTYLE_INVISIBLE);

    m_left->Add(m_tree, 1, wxEXPAND);
    m_left->Add(new wxStaticLine(), 0);
    m_left->Add(m_grid, 1, wxEXPAND);

    m_columns->Add(m_left, 1, wxEXPAND);
    m_columns->Add(m_editor, 1, wxEXPAND);

    m_window->SetSizer(m_columns);
}

void App::OnDropFiles(wxDropFilesEvent& event)
{
    if(event.GetNumberOfFiles() > 0)
    {
        delete m_ast;
        m_tree->DeleteAllItems();
        m_editor->ClearAll();

        wxString filename = event.GetFiles()[0];

        wxTreeItemId root_id = m_tree->AddRoot(filename);

        m_ast = new Ast();
        create_ast(m_ast, filename.c_str());

        BuildTree(m_ast, root_id);
        FillEditor(m_ast);
    }
}

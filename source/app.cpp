#include "app.hpp"

#include <wx/dir.h>
#include <wx/statline.h>

App::App(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxApp()
{
}

void App::BuildTree(Ast* ast, wxTreeItemId id)
{
    AppendTreeItems(id, ast->statements);

    if(ast->child)
    {
        wxTreeItemId child = m_tree->AppendItem(id, "child");
        BuildTree(ast->child, child);
    }
}

void App::AppendTreeItems(wxTreeItemId parent, Vector<Statement> statements)
{
    for(const auto& element : statements)
    {
        std::visit([this, &parent](auto&& s) { AppendTreeItem(parent, s); }, element);
    }
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Closure closure)
{
    auto child = m_tree->AppendItem(parent, "Closure");

    for(const auto& arg : closure.arguments)
    {
        AppendTreeItem(child, arg);
    }

    for(const auto& loc : closure.locals)
    {
        AppendTreeItem(child, loc);
    }

    AppendTreeItems(child, closure.statements);

    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Identifier identifier)
{
    auto child = m_tree->AppendItem(parent, "Identifier");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstInt number)
{
    auto child = m_tree->AppendItem(parent, "AstInt");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstList list)
{
    auto child = m_tree->AppendItem(parent, "AstList");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstMap map)
{
    auto child = m_tree->AppendItem(parent, "AstMap");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstNumber number)
{
    auto child = m_tree->AppendItem(parent, "AstNumber");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstOperation operation)
{
    auto child = m_tree->AppendItem(parent, "AstOperation");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstString string)
{
    auto child = m_tree->AppendItem(parent, "AstString");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, AstTable table)
{
    auto child = m_tree->AppendItem(parent, "AstTable");
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Assignment ass)
{
    auto child = m_tree->AppendItem(parent, "Assignment");
    m_tree->AppendItem(child, ass.left.name);
    std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, ass.right);
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Call call)
{
    auto child = m_tree->AppendItem(parent, "Call");
    m_tree->AppendItem(child, call.name.name);
    for(const auto& arg : call.arguments)
    {
        std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, arg);
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Condition condition)
{
    auto child = m_tree->AppendItem(parent, "Condition");
    for(const auto& block : condition.blocks)
    {
        AppendTreeItem(child, block.comparison);
        AppendTreeItems(child, block.statements);
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, ForLoop loop)
{
    auto child = m_tree->AppendItem(parent, "ForLoop");
    m_tree->AppendItem(child, loop.counter);
    std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, loop.begin);
    std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, loop.end);
    std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, loop.increment);
    AppendTreeItems(child, loop.statements);
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, ForInLoop loop)
{
    auto child = m_tree->AppendItem(parent, "ForInLoop");
    m_tree->AppendItem(child, loop.key);
    m_tree->AppendItem(child, loop.value);
    m_tree->AppendItem(child, loop.right);
    AppendTreeItems(child, loop.statements);
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, LocalAssignment ass)
{
    auto child = m_tree->AppendItem(parent, "LocalAssignment");
    m_tree->AppendItem(child, ass.left.name);
    std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, ass.right);
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, Return ret)
{
    auto child = m_tree->AppendItem(parent, "Return");
    for(const auto& e : ret.ex)
    {
        std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, e);
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, TailCall call)
{
    auto child = m_tree->AppendItem(parent, "TailCall");
    m_tree->AppendItem(child, call.name.name);
    for(const auto& arg : call.arguments)
    {
        std::visit([this, &child](auto&& s) { AppendTreeItem(child, s); }, arg);
    }
    return child;
}

wxTreeItemId App::AppendTreeItem(wxTreeItemId parent, WhileLoop loop)
{
    auto child = m_tree->AppendItem(parent, "WhileLoop");
    AppendTreeItem(child, loop.condition);
    AppendTreeItems(child, loop.statements);
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
        wxSize(1600, 1024),
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
    m_editor->SetReadOnly(true);
    m_editor->SetMarginType(1, wxSTC_MARGIN_NUMBER);
    m_editor->SetMarginMask(1, 0);
    m_editor->SetMarginWidth(1, 20);

    m_left->Add(m_tree, 1, wxEXPAND);
    m_left->Add(new wxStaticLine(), 0);
    m_left->Add(m_grid, 1, wxEXPAND);

    m_columns->Add(m_left, 1, wxEXPAND);
    m_columns->Add(m_editor, 1, wxEXPAND);

    m_window->SetSizer(m_columns);

    m_drop_hint = new wxStaticText(
        m_window,
        wxID_ANY,
        "Drop a compiled Lua 4.0 script anywhere",
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_FRAME_STYLE | wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);

    wxFont font = m_drop_hint->GetFont();
    font.SetPointSize(14);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    m_drop_hint->SetFont(font);

    m_drop_hint->SetSize(240, 60);
    m_drop_hint->Center();
}

void App::OnDropFiles(wxDropFilesEvent& event)
{
    if(event.GetNumberOfFiles() > 0)
    {
        delete m_ast;
        m_tree->DeleteAllItems();
        m_editor->SetReadOnly(false);
        m_editor->ClearAll();
        m_drop_hint->Hide();

        wxString filename = event.GetFiles()[0];

        wxTreeItemId root_id = m_tree->AddRoot(filename);

        m_ast = new Ast();
        create_ast(m_ast, filename.c_str());

        BuildTree(m_ast, root_id);
        m_tree->ExpandAllChildren(root_id);

        FillEditor(m_ast);
        m_editor->SetReadOnly(true);
    }
}

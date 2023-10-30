#include "app.hpp"
#include "parser/parser.hpp"

#include <wx/dir.h>
#include <wx/statline.h>

App::App(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxApp()
{
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
    m_tree->SetWindowStyle(m_tree->GetWindowStyle() | wxBORDER_NONE);

    m_grid = new wxPropertyGrid(m_window, wxID_ANY);
    m_grid->SetWindowStyle(m_grid->GetWindowStyle() | wxBORDER_NONE);
    wxArrayString choices;
    choices.Add("INT");
    choices.Add("FLOAT");
    m_grid->Append(new wxEnumProperty("Type", wxPG_LABEL, choices));

    m_editor = new wxStyledTextCtrl(m_window, wxID_ANY);
    m_editor->SetWindowStyle(m_editor->GetWindowStyle() | wxBORDER_NONE);
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
        wxString filename = event.GetFiles()[0];

        wxTreeItemId root_id = m_tree->AddRoot(filename);
        m_tree->AppendItem(root_id, "Test");

        Collection<byte> buffer;
        auto*            iter = buffer.data();

        auto chunk = read_chunk(iter);
    }
}

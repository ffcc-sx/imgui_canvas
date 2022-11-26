# include "imgui_node_editor_internal.h"
# include <algorithm>

static ImGuiEx::internal::Detail::EditorContext *s_Editor = nullptr;

template<typename C, typename I, typename F>
static int BuildIdList(C &container, I *list, int listSize, F &&accept) {
    if (list != nullptr) {
        int count = 0;
        for (auto object: container) {
            if (listSize <= 0)
                break;
            if (accept(object)) {
                list[count] = I(object->ID().AsPointer());
                ++count;
                --listSize;
            }
        }
        return count;
    } else
        return static_cast<int>(std::count_if(container.begin(), container.end(), accept));
}

//------------------------------------------------------------------------------
ImGuiEx::internal::EditorContext *ImGuiEx::internal::CreateEditor(const Config *config) {
    return reinterpret_cast<ImGuiEx::internal::EditorContext *>(new ImGuiEx::internal::Detail::EditorContext(config));
}

void ImGuiEx::internal::DestroyEditor(EditorContext *ctx) {
    auto lastContext = GetCurrentEditor();

    // Set context we're about to destroy as current, to give callback valid context
    if (lastContext != ctx)
        SetCurrentEditor(ctx);

    auto editor = reinterpret_cast<ImGuiEx::internal::Detail::EditorContext *>(ctx);

    delete editor;

    if (lastContext != ctx)
        SetCurrentEditor(lastContext);
}

const ImGuiEx::internal::Config &ImGuiEx::internal::GetConfig(EditorContext *ctx) {
    if (ctx == nullptr)
        ctx = GetCurrentEditor();

    if (ctx) {
        auto editor = reinterpret_cast<ImGuiEx::internal::Detail::EditorContext *>(ctx);

        return editor->GetConfig();
    } else {
        static Config s_EmptyConfig;
        return s_EmptyConfig;
    }
}

void ImGuiEx::internal::SetCurrentEditor(EditorContext *ctx) {
    s_Editor = reinterpret_cast<ImGuiEx::internal::Detail::EditorContext *>(ctx);
}

ImGuiEx::internal::EditorContext *ImGuiEx::internal::GetCurrentEditor() {
    return reinterpret_cast<ImGuiEx::internal::EditorContext *>(s_Editor);
}

ImGuiEx::internal::Style &ImGuiEx::internal::GetStyle() {
    return s_Editor->GetStyle();
}

const char *ImGuiEx::internal::GetStyleColorName(StyleColor colorIndex) {
    return s_Editor->GetStyle().GetColorName(colorIndex);
}

void ImGuiEx::internal::PushStyleColor(StyleColor colorIndex, const ImVec4 &color) {
    s_Editor->GetStyle().PushColor(colorIndex, color);
}

void ImGuiEx::internal::PopStyleColor(int count) {
    s_Editor->GetStyle().PopColor(count);
}

void ImGuiEx::internal::PushStyleVar(StyleVar varIndex, float value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::internal::PushStyleVar(StyleVar varIndex, const ImVec2 &value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::internal::PushStyleVar(StyleVar varIndex, const ImVec4 &value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::internal::PopStyleVar(int count) {
    s_Editor->GetStyle().PopVar(count);
}

void ImGuiEx::internal::Begin(const char *id, const ImVec2 &size) {
    s_Editor->Begin(id, size);
}

void ImGuiEx::internal::End() {
    s_Editor->End();
}

void ImGuiEx::internal::BeginNode(NodeId id) {
    s_Editor->GetNodeBuilder().Begin(id);
}

void ImGuiEx::internal::BeginPin(PinId id, PinKind kind) {
    s_Editor->GetNodeBuilder().BeginPin(id, kind);
}

void ImGuiEx::internal::PinRect(const ImVec2 &a, const ImVec2 &b) {
    s_Editor->GetNodeBuilder().PinRect(a, b);
}

void ImGuiEx::internal::PinPivotRect(const ImVec2 &a, const ImVec2 &b) {
    s_Editor->GetNodeBuilder().PinPivotRect(a, b);
}

void ImGuiEx::internal::PinPivotSize(const ImVec2 &size) {
    s_Editor->GetNodeBuilder().PinPivotSize(size);
}

void ImGuiEx::internal::PinPivotScale(const ImVec2 &scale) {
    s_Editor->GetNodeBuilder().PinPivotScale(scale);
}

void ImGuiEx::internal::PinPivotAlignment(const ImVec2 &alignment) {
    s_Editor->GetNodeBuilder().PinPivotAlignment(alignment);
}

void ImGuiEx::internal::EndPin() {
    s_Editor->GetNodeBuilder().EndPin();
}

void ImGuiEx::internal::Group(const ImVec2 &size) {
    s_Editor->GetNodeBuilder().Group(size);
}

void ImGuiEx::internal::EndNode() {
    s_Editor->GetNodeBuilder().End();
}

bool ImGuiEx::internal::BeginGroupHint(NodeId nodeId) {
    return s_Editor->GetHintBuilder().Begin(nodeId);
}

ImVec2 ImGuiEx::internal::GetGroupMin() {
    return s_Editor->GetHintBuilder().GetGroupMin();
}

ImVec2 ImGuiEx::internal::GetGroupMax() {
    return s_Editor->GetHintBuilder().GetGroupMax();
}

ImDrawList *ImGuiEx::internal::GetHintForegroundDrawList() {
    return s_Editor->GetHintBuilder().GetForegroundDrawList();
}

ImDrawList *ImGuiEx::internal::GetHintBackgroundDrawList() {
    return s_Editor->GetHintBuilder().GetBackgroundDrawList();
}

void ImGuiEx::internal::EndGroupHint() {
    s_Editor->GetHintBuilder().End();
}

ImDrawList *ImGuiEx::internal::GetNodeBackgroundDrawList(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->GetNodeBuilder().GetUserBackgroundDrawList(node);
    else
        return nullptr;
}

bool ImGuiEx::internal::Link(LinkId id, PinId startPinId, PinId endPinId, const ImVec4 &color/* = ImVec4(1, 1, 1, 1)*/,
                             float thickness/* = 1.0f*/) {
    return s_Editor->DoLink(id, startPinId, endPinId, ImColor(color), thickness);
}

void ImGuiEx::internal::Flow(LinkId linkId, FlowDirection direction) {
    if (auto link = s_Editor->FindLink(linkId))
        s_Editor->Flow(link, direction);
}

bool ImGuiEx::internal::BeginCreate(const ImVec4 &color, float thickness) {
    auto &context = s_Editor->GetItemCreator();

    if (context.Begin()) {
        context.SetStyle(ImColor(color), thickness);
        return true;
    } else
        return false;
}

bool ImGuiEx::internal::QueryNewLink(PinId *startId, PinId *endId) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.QueryLink(startId, endId) == Result::True;
}

bool ImGuiEx::internal::QueryNewLink(PinId *startId, PinId *endId, const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.QueryLink(startId, endId);
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

bool ImGuiEx::internal::QueryNewNode(PinId *pinId) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.QueryNode(pinId) == Result::True;
}

bool ImGuiEx::internal::QueryNewNode(PinId *pinId, const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.QueryNode(pinId);
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

bool ImGuiEx::internal::AcceptNewItem() {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.AcceptItem() == Result::True;
}

bool ImGuiEx::internal::AcceptNewItem(const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.AcceptItem();
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

void ImGuiEx::internal::RejectNewItem() {
    auto &context = s_Editor->GetItemCreator();

    context.RejectItem();
}

void ImGuiEx::internal::RejectNewItem(const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::internal::Detail::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    if (context.RejectItem() != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);
}

void ImGuiEx::internal::EndCreate() {
    auto &context = s_Editor->GetItemCreator();

    context.End();
}

bool ImGuiEx::internal::BeginDelete() {
    auto &context = s_Editor->GetItemDeleter();

    return context.Begin();
}

bool ImGuiEx::internal::QueryDeletedLink(LinkId *linkId, PinId *startId, PinId *endId) {
    auto &context = s_Editor->GetItemDeleter();

    return context.QueryLink(linkId, startId, endId);
}

bool ImGuiEx::internal::QueryDeletedNode(NodeId *nodeId) {
    auto &context = s_Editor->GetItemDeleter();

    return context.QueryNode(nodeId);
}

bool ImGuiEx::internal::AcceptDeletedItem(bool deleteDependencies) {
    auto &context = s_Editor->GetItemDeleter();

    return context.AcceptItem(deleteDependencies);
}

void ImGuiEx::internal::RejectDeletedItem() {
    auto &context = s_Editor->GetItemDeleter();

    context.RejectItem();
}

void ImGuiEx::internal::EndDelete() {
    auto &context = s_Editor->GetItemDeleter();

    context.End();
}

void ImGuiEx::internal::SetNodePosition(NodeId nodeId, const ImVec2 &position) {
    s_Editor->SetNodePosition(nodeId, position);
}

void ImGuiEx::internal::SetGroupSize(NodeId nodeId, const ImVec2 &size) {
    s_Editor->SetGroupSize(nodeId, size);
}

ImVec2 ImGuiEx::internal::GetNodePosition(NodeId nodeId) {
    return s_Editor->GetNodePosition(nodeId);
}

ImVec2 ImGuiEx::internal::GetNodeSize(NodeId nodeId) {
    return s_Editor->GetNodeSize(nodeId);
}

void ImGuiEx::internal::CenterNodeOnScreen(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        node->CenterOnScreenInNextFrame();
}

void ImGuiEx::internal::SetNodeZPosition(NodeId nodeId, float z) {
    s_Editor->SetNodeZPosition(nodeId, z);
}

float ImGuiEx::internal::GetNodeZPosition(NodeId nodeId) {
    return s_Editor->GetNodeZPosition(nodeId);
}

void ImGuiEx::internal::RestoreNodeState(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        s_Editor->MarkNodeToRestoreState(node);
}

void ImGuiEx::internal::Suspend() {
    s_Editor->Suspend();
}

void ImGuiEx::internal::Resume() {
    s_Editor->Resume();
}

bool ImGuiEx::internal::IsSuspended() {
    return s_Editor->IsSuspended();
}

bool ImGuiEx::internal::IsActive() {
    return s_Editor->IsFocused();
}

bool ImGuiEx::internal::HasSelectionChanged() {
    return s_Editor->HasSelectionChanged();
}

int ImGuiEx::internal::GetSelectedObjectCount() {
    return (int) s_Editor->GetSelectedObjects().size();
}

int ImGuiEx::internal::GetSelectedNodes(NodeId *nodes, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), nodes, size, [](auto object) {
        return object->AsNode() != nullptr;
    });
}

int ImGuiEx::internal::GetSelectedLinks(LinkId *links, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), links, size, [](auto object) {
        return object->AsLink() != nullptr;
    });
}

bool ImGuiEx::internal::IsNodeSelected(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->IsSelected(node);
    else
        return false;
}

bool ImGuiEx::internal::IsLinkSelected(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        return s_Editor->IsSelected(link);
    else
        return false;
}

void ImGuiEx::internal::ClearSelection() {
    s_Editor->ClearSelection();
}

void ImGuiEx::internal::SelectNode(NodeId nodeId, bool append) {
    if (auto node = s_Editor->FindNode(nodeId)) {
        if (append)
            s_Editor->SelectObject(node);
        else
            s_Editor->SetSelectedObject(node);
    }
}

void ImGuiEx::internal::SelectLink(LinkId linkId, bool append) {
    if (auto link = s_Editor->FindLink(linkId)) {
        if (append)
            s_Editor->SelectObject(link);
        else
            s_Editor->SetSelectedObject(link);
    }
}

void ImGuiEx::internal::DeselectNode(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        s_Editor->DeselectObject(node);
}

void ImGuiEx::internal::DeselectLink(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        s_Editor->DeselectObject(link);
}

bool ImGuiEx::internal::DeleteNode(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->GetItemDeleter().Add(node);
    else
        return false;
}

bool ImGuiEx::internal::DeleteLink(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        return s_Editor->GetItemDeleter().Add(link);
    else
        return false;
}

bool ImGuiEx::internal::HasAnyLinks(NodeId nodeId) {
    return s_Editor->HasAnyLinks(nodeId);
}

bool ImGuiEx::internal::HasAnyLinks(PinId pinId) {
    return s_Editor->HasAnyLinks(pinId);
}

int ImGuiEx::internal::BreakLinks(NodeId nodeId) {
    return s_Editor->BreakLinks(nodeId);
}

int ImGuiEx::internal::BreakLinks(PinId pinId) {
    return s_Editor->BreakLinks(pinId);
}

void ImGuiEx::internal::NavigateToContent(float duration) {
    s_Editor->NavigateTo(s_Editor->GetContentBounds(), true, duration);
}

void ImGuiEx::internal::NavigateToSelection(bool zoomIn, float duration) {
    s_Editor->NavigateTo(s_Editor->GetSelectionBounds(), zoomIn, duration);
}

bool ImGuiEx::internal::ShowNodeContextMenu(NodeId *nodeId) {
    return s_Editor->GetContextMenu().ShowNodeContextMenu(nodeId);
}

bool ImGuiEx::internal::ShowPinContextMenu(PinId *pinId) {
    return s_Editor->GetContextMenu().ShowPinContextMenu(pinId);
}

bool ImGuiEx::internal::ShowLinkContextMenu(LinkId *linkId) {
    return s_Editor->GetContextMenu().ShowLinkContextMenu(linkId);
}

bool ImGuiEx::internal::ShowBackgroundContextMenu() {
    return s_Editor->GetContextMenu().ShowBackgroundContextMenu();
}

void ImGuiEx::internal::EnableShortcuts(bool enable) {
    s_Editor->EnableShortcuts(enable);
}

bool ImGuiEx::internal::AreShortcutsEnabled() {
    return s_Editor->AreShortcutsEnabled();
}

bool ImGuiEx::internal::BeginShortcut() {
    return s_Editor->GetShortcut().Begin();
}

bool ImGuiEx::internal::AcceptCut() {
    return s_Editor->GetShortcut().AcceptCut();
}

bool ImGuiEx::internal::AcceptCopy() {
    return s_Editor->GetShortcut().AcceptCopy();
}

bool ImGuiEx::internal::AcceptPaste() {
    return s_Editor->GetShortcut().AcceptPaste();
}

bool ImGuiEx::internal::AcceptDuplicate() {
    return s_Editor->GetShortcut().AcceptDuplicate();
}

bool ImGuiEx::internal::AcceptCreateNode() {
    return s_Editor->GetShortcut().AcceptCreateNode();
}

int ImGuiEx::internal::GetActionContextSize() {
    return static_cast<int>(s_Editor->GetShortcut().m_Context.size());
}

int ImGuiEx::internal::GetActionContextNodes(NodeId *nodes, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), nodes, size, [](auto object) {
        return object->AsNode() != nullptr;
    });
}

int ImGuiEx::internal::GetActionContextLinks(LinkId *links, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), links, size, [](auto object) {
        return object->AsLink() != nullptr;
    });
}

void ImGuiEx::internal::EndShortcut() {
    return s_Editor->GetShortcut().End();
}

float ImGuiEx::internal::GetCurrentZoom() {
    return s_Editor->GetView().InvScale;
}

ImGuiEx::internal::NodeId ImGuiEx::internal::GetHoveredNode() {
    return s_Editor->GetHoveredNode();
}

ImGuiEx::internal::PinId ImGuiEx::internal::GetHoveredPin() {
    return s_Editor->GetHoveredPin();
}

ImGuiEx::internal::LinkId ImGuiEx::internal::GetHoveredLink() {
    return s_Editor->GetHoveredLink();
}

ImGuiEx::internal::NodeId ImGuiEx::internal::GetDoubleClickedNode() {
    return s_Editor->GetDoubleClickedNode();
}

ImGuiEx::internal::PinId ImGuiEx::internal::GetDoubleClickedPin() {
    return s_Editor->GetDoubleClickedPin();
}

ImGuiEx::internal::LinkId ImGuiEx::internal::GetDoubleClickedLink() {
    return s_Editor->GetDoubleClickedLink();
}

bool ImGuiEx::internal::IsBackgroundClicked() {
    return s_Editor->IsBackgroundClicked();
}

bool ImGuiEx::internal::IsBackgroundDoubleClicked() {
    return s_Editor->IsBackgroundDoubleClicked();
}

ImGuiMouseButton ImGuiEx::internal::GetBackgroundClickButtonIndex() {
    return s_Editor->GetBackgroundClickButtonIndex();
}

ImGuiMouseButton ImGuiEx::internal::GetBackgroundDoubleClickButtonIndex() {
    return s_Editor->GetBackgroundDoubleClickButtonIndex();
}

bool ImGuiEx::internal::GetLinkPins(LinkId linkId, PinId *startPinId, PinId *endPinId) {
    auto link = s_Editor->FindLink(linkId);
    if (!link)
        return false;

    if (startPinId)
        *startPinId = link->m_StartPin->m_ID;
    if (endPinId)
        *endPinId = link->m_EndPin->m_ID;

    return true;
}

bool ImGuiEx::internal::PinHadAnyLinks(PinId pinId) {
    return s_Editor->PinHadAnyLinks(pinId);
}

ImVec2 ImGuiEx::internal::GetScreenSize() {
    return s_Editor->GetRect().GetSize();
}

ImVec2 ImGuiEx::internal::ScreenToCanvas(const ImVec2 &pos) {
    return s_Editor->ToCanvas(pos);
}

ImVec2 ImGuiEx::internal::CanvasToScreen(const ImVec2 &pos) {
    return s_Editor->ToScreen(pos);
}

int ImGuiEx::internal::GetNodeCount() {
    return s_Editor->CountLiveNodes();
}

int ImGuiEx::internal::GetOrderedNodeIds(NodeId *nodes, int size) {
    return s_Editor->GetNodeIds(nodes, size);
}

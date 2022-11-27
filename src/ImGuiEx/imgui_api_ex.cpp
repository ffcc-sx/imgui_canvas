# include "internal/internal_private.h"
#include "internal/internal_api.h"
# include <algorithm>

static ImGuiEx::api::internal::EditorContext *s_Editor = nullptr;

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
ImGuiEx::api::EditorContext *ImGuiEx::api::CreateEditor(const Config *config) {
    return reinterpret_cast<ImGuiEx::api::EditorContext *>(new ImGuiEx::api::internal::EditorContext(config));
}

void ImGuiEx::api::DestroyEditor(EditorContext *ctx) {
    auto lastContext = GetCurrentEditor();

    // Set context we're about to destroy as current, to give callback valid context
    if (lastContext != ctx)
        SetCurrentEditor(ctx);

    auto editor = reinterpret_cast<ImGuiEx::api::internal::EditorContext *>(ctx);

    delete editor;

    if (lastContext != ctx)
        SetCurrentEditor(lastContext);
}

const ImGuiEx::api::Config &ImGuiEx::api::GetConfig(EditorContext *ctx) {
    if (ctx == nullptr)
        ctx = GetCurrentEditor();

    if (ctx) {
        auto editor = reinterpret_cast<ImGuiEx::api::internal::EditorContext *>(ctx);

        return editor->GetConfig();
    } else {
        static Config s_EmptyConfig;
        return s_EmptyConfig;
    }
}

void ImGuiEx::api::SetCurrentEditor(EditorContext *ctx) {
    s_Editor = reinterpret_cast<ImGuiEx::api::internal::EditorContext *>(ctx);
}

ImGuiEx::api::EditorContext *ImGuiEx::api::GetCurrentEditor() {
    return reinterpret_cast<ImGuiEx::api::EditorContext *>(s_Editor);
}

ImGuiEx::api::Style &ImGuiEx::api::GetStyle() {
    return s_Editor->GetStyle();
}

const char *ImGuiEx::api::GetStyleColorName(StyleColor colorIndex) {
    return s_Editor->GetStyle().GetColorName(colorIndex);
}

void ImGuiEx::api::PushStyleColor(StyleColor colorIndex, const ImVec4 &color) {
    s_Editor->GetStyle().PushColor(colorIndex, color);
}

void ImGuiEx::api::PopStyleColor(int count) {
    s_Editor->GetStyle().PopColor(count);
}

void ImGuiEx::api::PushStyleVar(StyleVar varIndex, float value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::api::PushStyleVar(StyleVar varIndex, const ImVec2 &value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::api::PushStyleVar(StyleVar varIndex, const ImVec4 &value) {
    s_Editor->GetStyle().PushVar(varIndex, value);
}

void ImGuiEx::api::PopStyleVar(int count) {
    s_Editor->GetStyle().PopVar(count);
}

void ImGuiEx::api::Begin(const char *id, const ImVec2 &size) {
    s_Editor->Begin(id, size);
}

void ImGuiEx::api::End() {
    s_Editor->End();
}

void ImGuiEx::api::BeginNode(NodeId id) {
    s_Editor->GetNodeBuilder().Begin(id);
}

void ImGuiEx::api::BeginPin(PinId id, PinKind kind) {
    s_Editor->GetNodeBuilder().BeginPin(id, kind);
}

void ImGuiEx::api::PinRect(const ImVec2 &a, const ImVec2 &b) {
    s_Editor->GetNodeBuilder().PinRect(a, b);
}

void ImGuiEx::api::PinPivotRect(const ImVec2 &a, const ImVec2 &b) {
    s_Editor->GetNodeBuilder().PinPivotRect(a, b);
}

void ImGuiEx::api::PinPivotSize(const ImVec2 &size) {
    s_Editor->GetNodeBuilder().PinPivotSize(size);
}

void ImGuiEx::api::PinPivotScale(const ImVec2 &scale) {
    s_Editor->GetNodeBuilder().PinPivotScale(scale);
}

void ImGuiEx::api::PinPivotAlignment(const ImVec2 &alignment) {
    s_Editor->GetNodeBuilder().PinPivotAlignment(alignment);
}

void ImGuiEx::api::EndPin() {
    s_Editor->GetNodeBuilder().EndPin();
}

void ImGuiEx::api::Group(const ImVec2 &size) {
    s_Editor->GetNodeBuilder().Group(size);
}

void ImGuiEx::api::EndNode() {
    s_Editor->GetNodeBuilder().End();
}

bool ImGuiEx::api::BeginGroupHint(NodeId nodeId) {
    return s_Editor->GetHintBuilder().Begin(nodeId);
}

ImVec2 ImGuiEx::api::GetGroupMin() {
    return s_Editor->GetHintBuilder().GetGroupMin();
}

ImVec2 ImGuiEx::api::GetGroupMax() {
    return s_Editor->GetHintBuilder().GetGroupMax();
}

ImDrawList *ImGuiEx::api::GetHintForegroundDrawList() {
    return s_Editor->GetHintBuilder().GetForegroundDrawList();
}

ImDrawList *ImGuiEx::api::GetHintBackgroundDrawList() {
    return s_Editor->GetHintBuilder().GetBackgroundDrawList();
}

void ImGuiEx::api::EndGroupHint() {
    s_Editor->GetHintBuilder().End();
}

ImDrawList *ImGuiEx::api::GetNodeBackgroundDrawList(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->GetNodeBuilder().GetUserBackgroundDrawList(node);
    else
        return nullptr;
}

bool ImGuiEx::api::Link(LinkId id, PinId startPinId, PinId endPinId, const ImVec4 &color/* = ImVec4(1, 1, 1, 1)*/,
                        float thickness/* = 1.0f*/) {
    return s_Editor->DoLink(id, startPinId, endPinId, ImColor(color), thickness);
}

void ImGuiEx::api::Flow(LinkId linkId, FlowDirection direction) {
    if (auto link = s_Editor->FindLink(linkId))
        s_Editor->Flow(link, direction);
}

bool ImGuiEx::api::BeginCreate(const ImVec4 &color, float thickness) {
    auto &context = s_Editor->GetItemCreator();

    if (context.Begin()) {
        context.SetStyle(ImColor(color), thickness);
        return true;
    } else
        return false;
}

bool ImGuiEx::api::QueryNewLink(PinId *startId, PinId *endId) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.QueryLink(startId, endId) == Result::True;
}

bool ImGuiEx::api::QueryNewLink(PinId *startId, PinId *endId, const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.QueryLink(startId, endId);
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

bool ImGuiEx::api::QueryNewNode(PinId *pinId) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.QueryNode(pinId) == Result::True;
}

bool ImGuiEx::api::QueryNewNode(PinId *pinId, const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.QueryNode(pinId);
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

bool ImGuiEx::api::AcceptNewItem() {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    return context.AcceptItem() == Result::True;
}

bool ImGuiEx::api::AcceptNewItem(const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    auto result = context.AcceptItem();
    if (result != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);

    return result == Result::True;
}

void ImGuiEx::api::RejectNewItem() {
    auto &context = s_Editor->GetItemCreator();

    context.RejectItem();
}

void ImGuiEx::api::RejectNewItem(const ImVec4 &color, float thickness) {
    using Result = ImGuiEx::api::internal::CreateItemAction::Result;

    auto &context = s_Editor->GetItemCreator();

    if (context.RejectItem() != Result::Indeterminate)
        context.SetStyle(ImColor(color), thickness);
}

void ImGuiEx::api::EndCreate() {
    auto &context = s_Editor->GetItemCreator();

    context.End();
}

bool ImGuiEx::api::BeginDelete() {
    auto &context = s_Editor->GetItemDeleter();

    return context.Begin();
}

bool ImGuiEx::api::QueryDeletedLink(LinkId *linkId, PinId *startId, PinId *endId) {
    auto &context = s_Editor->GetItemDeleter();

    return context.QueryLink(linkId, startId, endId);
}

bool ImGuiEx::api::QueryDeletedNode(NodeId *nodeId) {
    auto &context = s_Editor->GetItemDeleter();

    return context.QueryNode(nodeId);
}

bool ImGuiEx::api::AcceptDeletedItem(bool deleteDependencies) {
    auto &context = s_Editor->GetItemDeleter();

    return context.AcceptItem(deleteDependencies);
}

void ImGuiEx::api::RejectDeletedItem() {
    auto &context = s_Editor->GetItemDeleter();

    context.RejectItem();
}

void ImGuiEx::api::EndDelete() {
    auto &context = s_Editor->GetItemDeleter();

    context.End();
}

void ImGuiEx::api::SetNodePosition(NodeId nodeId, const ImVec2 &position) {
    s_Editor->SetNodePosition(nodeId, position);
}

void ImGuiEx::api::SetGroupSize(NodeId nodeId, const ImVec2 &size) {
    s_Editor->SetGroupSize(nodeId, size);
}

ImVec2 ImGuiEx::api::GetNodePosition(NodeId nodeId) {
    return s_Editor->GetNodePosition(nodeId);
}

ImVec2 ImGuiEx::api::GetNodeSize(NodeId nodeId) {
    return s_Editor->GetNodeSize(nodeId);
}

void ImGuiEx::api::CenterNodeOnScreen(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        node->CenterOnScreenInNextFrame();
}

void ImGuiEx::api::SetNodeZPosition(NodeId nodeId, float z) {
    s_Editor->SetNodeZPosition(nodeId, z);
}

float ImGuiEx::api::GetNodeZPosition(NodeId nodeId) {
    return s_Editor->GetNodeZPosition(nodeId);
}

void ImGuiEx::api::RestoreNodeState(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        s_Editor->MarkNodeToRestoreState(node);
}

void ImGuiEx::api::Suspend() {
    s_Editor->Suspend();
}

void ImGuiEx::api::Resume() {
    s_Editor->Resume();
}

bool ImGuiEx::api::IsSuspended() {
    return s_Editor->IsSuspended();
}

bool ImGuiEx::api::IsActive() {
    return s_Editor->IsFocused();
}

bool ImGuiEx::api::HasSelectionChanged() {
    return s_Editor->HasSelectionChanged();
}

int ImGuiEx::api::GetSelectedObjectCount() {
    return (int) s_Editor->GetSelectedObjects().size();
}

int ImGuiEx::api::GetSelectedNodes(NodeId *nodes, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), nodes, size, [](auto object) {
        return object->AsNode() != nullptr;
    });
}

int ImGuiEx::api::GetSelectedLinks(LinkId *links, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), links, size, [](auto object) {
        return object->AsLink() != nullptr;
    });
}

bool ImGuiEx::api::IsNodeSelected(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->IsSelected(node);
    else
        return false;
}

bool ImGuiEx::api::IsLinkSelected(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        return s_Editor->IsSelected(link);
    else
        return false;
}

void ImGuiEx::api::ClearSelection() {
    s_Editor->ClearSelection();
}

void ImGuiEx::api::SelectNode(NodeId nodeId, bool append) {
    if (auto node = s_Editor->FindNode(nodeId)) {
        if (append)
            s_Editor->SelectObject(node);
        else
            s_Editor->SetSelectedObject(node);
    }
}

void ImGuiEx::api::SelectLink(LinkId linkId, bool append) {
    if (auto link = s_Editor->FindLink(linkId)) {
        if (append)
            s_Editor->SelectObject(link);
        else
            s_Editor->SetSelectedObject(link);
    }
}

void ImGuiEx::api::DeselectNode(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        s_Editor->DeselectObject(node);
}

void ImGuiEx::api::DeselectLink(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        s_Editor->DeselectObject(link);
}

bool ImGuiEx::api::DeleteNode(NodeId nodeId) {
    if (auto node = s_Editor->FindNode(nodeId))
        return s_Editor->GetItemDeleter().Add(node);
    else
        return false;
}

bool ImGuiEx::api::DeleteLink(LinkId linkId) {
    if (auto link = s_Editor->FindLink(linkId))
        return s_Editor->GetItemDeleter().Add(link);
    else
        return false;
}

bool ImGuiEx::api::HasAnyLinks(NodeId nodeId) {
    return s_Editor->HasAnyLinks(nodeId);
}

bool ImGuiEx::api::HasAnyLinks(PinId pinId) {
    return s_Editor->HasAnyLinks(pinId);
}

int ImGuiEx::api::BreakLinks(NodeId nodeId) {
    return s_Editor->BreakLinks(nodeId);
}

int ImGuiEx::api::BreakLinks(PinId pinId) {
    return s_Editor->BreakLinks(pinId);
}

void ImGuiEx::api::NavigateToContent(float duration) {
    s_Editor->NavigateTo(s_Editor->GetContentBounds(), true, duration);
}

void ImGuiEx::api::NavigateToSelection(bool zoomIn, float duration) {
    s_Editor->NavigateTo(s_Editor->GetSelectionBounds(), zoomIn, duration);
}

bool ImGuiEx::api::ShowNodeContextMenu(NodeId *nodeId) {
    return s_Editor->GetContextMenu().ShowNodeContextMenu(nodeId);
}

bool ImGuiEx::api::ShowPinContextMenu(PinId *pinId) {
    return s_Editor->GetContextMenu().ShowPinContextMenu(pinId);
}

bool ImGuiEx::api::ShowLinkContextMenu(LinkId *linkId) {
    return s_Editor->GetContextMenu().ShowLinkContextMenu(linkId);
}

bool ImGuiEx::api::ShowBackgroundContextMenu() {
    return s_Editor->GetContextMenu().ShowBackgroundContextMenu();
}

void ImGuiEx::api::EnableShortcuts(bool enable) {
    s_Editor->EnableShortcuts(enable);
}

bool ImGuiEx::api::AreShortcutsEnabled() {
    return s_Editor->AreShortcutsEnabled();
}

bool ImGuiEx::api::BeginShortcut() {
    return s_Editor->GetShortcut().Begin();
}

bool ImGuiEx::api::AcceptCut() {
    return s_Editor->GetShortcut().AcceptCut();
}

bool ImGuiEx::api::AcceptCopy() {
    return s_Editor->GetShortcut().AcceptCopy();
}

bool ImGuiEx::api::AcceptPaste() {
    return s_Editor->GetShortcut().AcceptPaste();
}

bool ImGuiEx::api::AcceptDuplicate() {
    return s_Editor->GetShortcut().AcceptDuplicate();
}

bool ImGuiEx::api::AcceptCreateNode() {
    return s_Editor->GetShortcut().AcceptCreateNode();
}

int ImGuiEx::api::GetActionContextSize() {
    return static_cast<int>(s_Editor->GetShortcut().m_Context.size());
}

int ImGuiEx::api::GetActionContextNodes(NodeId *nodes, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), nodes, size, [](auto object) {
        return object->AsNode() != nullptr;
    });
}

int ImGuiEx::api::GetActionContextLinks(LinkId *links, int size) {
    return BuildIdList(s_Editor->GetSelectedObjects(), links, size, [](auto object) {
        return object->AsLink() != nullptr;
    });
}

void ImGuiEx::api::EndShortcut() {
    return s_Editor->GetShortcut().End();
}

float ImGuiEx::api::GetCurrentZoom() {
    return s_Editor->GetView().InvScale;
}

ImGuiEx::api::NodeId ImGuiEx::api::GetHoveredNode() {
    return s_Editor->GetHoveredNode();
}

ImGuiEx::api::PinId ImGuiEx::api::GetHoveredPin() {
    return s_Editor->GetHoveredPin();
}

ImGuiEx::api::LinkId ImGuiEx::api::GetHoveredLink() {
    return s_Editor->GetHoveredLink();
}

ImGuiEx::api::NodeId ImGuiEx::api::GetDoubleClickedNode() {
    return s_Editor->GetDoubleClickedNode();
}

ImGuiEx::api::PinId ImGuiEx::api::GetDoubleClickedPin() {
    return s_Editor->GetDoubleClickedPin();
}

ImGuiEx::api::LinkId ImGuiEx::api::GetDoubleClickedLink() {
    return s_Editor->GetDoubleClickedLink();
}

bool ImGuiEx::api::IsBackgroundClicked() {
    return s_Editor->IsBackgroundClicked();
}

bool ImGuiEx::api::IsBackgroundDoubleClicked() {
    return s_Editor->IsBackgroundDoubleClicked();
}

ImGuiMouseButton ImGuiEx::api::GetBackgroundClickButtonIndex() {
    return s_Editor->GetBackgroundClickButtonIndex();
}

ImGuiMouseButton ImGuiEx::api::GetBackgroundDoubleClickButtonIndex() {
    return s_Editor->GetBackgroundDoubleClickButtonIndex();
}

bool ImGuiEx::api::GetLinkPins(LinkId linkId, PinId *startPinId, PinId *endPinId) {
    auto link = s_Editor->FindLink(linkId);
    if (!link)
        return false;

    if (startPinId)
        *startPinId = link->m_StartPin->m_ID;
    if (endPinId)
        *endPinId = link->m_EndPin->m_ID;

    return true;
}

bool ImGuiEx::api::PinHadAnyLinks(PinId pinId) {
    return s_Editor->PinHadAnyLinks(pinId);
}

ImVec2 ImGuiEx::api::GetScreenSize() {
    return s_Editor->GetRect().GetSize();
}

ImVec2 ImGuiEx::api::ScreenToCanvas(const ImVec2 &pos) {
    return s_Editor->ToCanvas(pos);
}

ImVec2 ImGuiEx::api::CanvasToScreen(const ImVec2 &pos) {
    return s_Editor->ToScreen(pos);
}

int ImGuiEx::api::GetNodeCount() {
    return s_Editor->CountLiveNodes();
}

int ImGuiEx::api::GetOrderedNodeIds(NodeId *nodes, int size) {
    return s_Editor->GetNodeIds(nodes, size);
}

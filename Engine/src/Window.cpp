#include "Window.h"
namespace SFT {

#pragma region Window Method Implimentations

#pragma region Window Properties Management

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::setTitle(const std::string& newTitle) {
        glfwSetWindowTitle(this->m_window_handle, newTitle.c_str());
    }

    void Window::setPosition(tvec2<size_t> position) {
        glfwSetWindowPos(this->m_window_handle, position.x, position.y);
    }

    tvec2<size_t> Window::getPosition() {
        int x, y;
        glfwGetWindowPos(this->m_window_handle, &x, &y);
        return tvec2<size_t>(x, y);
    }

    GLFWwindow* Window::getHandle() {
        return this->m_window_handle;
    }

    bool Window::shouldWindowClose() {
        return glfwWindowShouldClose(this->m_window_handle);
    }

    void Window::requestFocus() {
        glfwRequestWindowAttention(this->m_window_handle);
    }

    void Window::enablePointerLock() {
        glfwSetInputMode(this->m_window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::disablePointerLock() {
        glfwSetInputMode(this->m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    tvec2<size_t> Window::getSize() {
        int x = 0, y = 0;
        glfwGetWindowSize(this->m_window_handle, &x, &y);
        return { x, y };
    }

#pragma endregion

#pragma region Event Handling

    void Window::callKeyHandlers(const input::KeyEvent& event) {
        for (auto& handler : this->m_key_handlers) {
            if (handler.active) {
                handler.handler(event);
            }
        }
    }

    void Window::callCursorPosHandlers(const input::CursorPositionEvent& event) {
        for (auto& handler : this->m_cursor_position_handlers) {
            if (handler.active) {
                handler.handler(event);
            }
        }
    }

    void Window::callClickHandlers(const input::ClickEvent& event) {
        for (auto& handler : this->m_click_handlers) {
            if (handler.active) {
                handler.handler(event);
            }
        }
    }

    void Window::callScrollHandlers(const input::ScrollEvent& event) {
        for (auto& handler : this->m_scroll_handlers) {
            if (handler.active) {
                handler.handler(event);
            }
        }
    }

#pragma endregion

#pragma region Handler Management

    size_t Window::addKeyHandler(input::KeyEventHandler handler) {
        input::KeyHandlerStore store{ handler, true };
        size_t size = this->m_key_handlers.size();
        this->m_key_handlers.push_back(store);
        return size;
    }

    size_t Window::addCursorPositionHandler(input::CursorPositionEventHandler handler) {
        input::CursorPositionHandlerStore store{ handler, true };
        size_t size = this->m_cursor_position_handlers.size();
        this->m_cursor_position_handlers.push_back(store);
        return size;
    }

    size_t Window::addClickHandler(input::ClickEventHandler handler) {
        input::ClickHandlerStore store{ handler, true };
        size_t len = this->m_click_handlers.size();
        this->m_click_handlers.push_back(store);
        return len;
    }

    size_t Window::addScrollHandler(input::ScrollEventHandler handler) {
        input::ScrollHandlerStore store{ handler, true };
        size_t len = this->m_scroll_handlers.size();
        this->m_scroll_handlers.push_back(store);
        return len;
    };

    void Window::deactivateKeyHandler(size_t index) {
        if (index < this->m_key_handlers.size()) {
            this->m_key_handlers[index].active = false;
        }
    }

    void Window::deactivateCursorPositionHandler(size_t index) {
        if (index < this->m_cursor_position_handlers.size()) {
            this->m_cursor_position_handlers[index].active = false;
        }
    }

    void Window::deactivateClickHandler(size_t index) {
        if (index < this->m_click_handlers.size()) {
            this->m_click_handlers[index].active = false;
        }
    }

    void Window::deactivateScrollHandler(size_t index) {
        if (index < this->m_scroll_handlers.size()) {
            this->m_click_handlers[index].active = false;
        }
    }

    void Window::activateKeyHandler(size_t index) {
        if (index < this->m_key_handlers.size()) {
            this->m_key_handlers[index].active = true;
        }
    }

    void Window::activateCursorPositionHandler(size_t index) {
        if (index < this->m_cursor_position_handlers.size()) {
            this->m_cursor_position_handlers[index].active = true;
        }
    }

    void Window::activateClickHandler(size_t index) {
        if (index < this->m_click_handlers.size()) {
            this->m_click_handlers[index].active = true;
        }
    }

    void Window::activateScrollHandler(size_t index) {
        if (index < this->m_scroll_handlers.size()) {
            this->m_scroll_handlers[index].active = true;
        }
    }

    

#pragma endregion

#pragma endregion

}

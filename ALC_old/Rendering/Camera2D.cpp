/*
* MIT License
*
* Copyright (c) 2021 Domara Shlimon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "Camera2D.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include "../Core/SceneManager.hpp"

namespace ALC {

	Camera2D::Camera2D() : m_position(0.0f), m_size(0.0f) {
		m_size = SceneManager::GetWindow()->GetScreenSize();
	}

	Camera2D::~Camera2D() { }

	vec2 Camera2D::GetPosition() {
		return m_position;
	}

	void Camera2D::SetPosition(const vec2& position_) {
		m_position = position_;
	}

	vec2 Camera2D::GetCameraSize() const {
		return m_size;
	}

	void Camera2D::SetCameraSize(const vec2& size_) {
		m_size = size_;
	}

	mat4 Camera2D::GetTransform() const {
		mat4 transform(1.0f);
		// create the view
		transform = glm::translate(transform, -vec3(m_position, 0.0f));

		// create the ortho
		vec2 halfsize = m_size * 0.5f;
		transform = transform * glm::ortho(-halfsize.x, halfsize.x, -halfsize.y, halfsize.y);

		// return the transform
		return transform;
	}

	mat4 Camera2D::GetScreenToWorld() const {
		return GetScreenToWorld(SceneManager::GetWindow()->GetScreenSize());
	}

	mat4 Camera2D::GetScreenToWorld(const vec2& screensize) const {
		mat4 screen = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y);
		return glm::inverse(GetTransform()) * screen;
	}

	mat4 Camera2D::GetWorldToScreen() const {
		return GetWorldToScreen(SceneManager::GetWindow()->GetScreenSize());
	}

	mat4 Camera2D::GetWorldToScreen(const vec2& screensize) const {
		mat4 screen = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y);
		return glm::inverse(screen) * GetTransform();
	}

}

#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt)
{
    if (m_Target != nullptr) {
		m_ViewBox.x = m_Target->X - SCREEN_WIDTH/2;
		m_ViewBox.y = m_Target->Y - SCREEN_HEIGHT/2;

        //khi nv di qua nua mh(theo ca truc X va Y) --> di chuyen cam

		if (m_ViewBox.x < 0) {
			m_ViewBox.x = 0;
		}

		if (m_ViewBox.y < 0) {
			m_ViewBox.y = 0;
		}

		//khi cam di chuyen ria phai map-> giu nguyen cam

		if (m_ViewBox.x > (2.665 * SCREEN_WIDTH - m_ViewBox.w)) {
			m_ViewBox.x = (2.665 * SCREEN_WIDTH - m_ViewBox.w);
		}

		if (m_ViewBox.y > (1 * SCREEN_HEIGHT - m_ViewBox.h)) {
			m_ViewBox.y = (1 * SCREEN_HEIGHT - m_ViewBox.h);
		}

		m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y); //lay vi tri cua cam
	}
}

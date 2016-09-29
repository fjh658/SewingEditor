#include <QEvent>
#include "SvgViewer.h"
#include "svgpp\SvgManager.h"
#include "svgpp\SvgAbstractObject.h"

#include "AbstractEventHandle.h"
#include "ShapeEventHandle.h"
#include "GroupEventHandle.h"
#include "ZoomEventHandle.h"

AbstractEventHandle::AbstractEventHandle(SvgViewer* v)
{
	m_viewer = v;
	m_lastHighlightShapeId = -1;
}

AbstractEventHandle::~AbstractEventHandle()
{

}

AbstractEventHandle* AbstractEventHandle::create(ProcessorType type, SvgViewer* v)
{
	switch (type)
	{
	case AbstractEventHandle::ProcessorTypeGeneral:
		return new AbstractEventHandle(v);
	case AbstractEventHandle::ProcessorTypeShape:
		return new ShapeEventHandle(v);
	case AbstractEventHandle::ProcessorTypeGroup:
		return new GroupEventHandle(v);
	case AbstractEventHandle::ProcessorTypeZoom:
		return new ZoomEventHandle(v);
	case AbstractEventHandle::ProcessorTypeEnd:
	default:
		return nullptr;
	}
}

void AbstractEventHandle::mousePressEvent(QMouseEvent *ev)
{

}

void AbstractEventHandle::mouseReleaseEvent(QMouseEvent *ev)
{

}

void AbstractEventHandle::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::MouseButton::MiddleButton)
	{
		m_viewer->resetCamera();
	}
}

void AbstractEventHandle::mouseMoveEvent(QMouseEvent *ev)
{
	if (m_viewer->buttons() == Qt::NoButton)
	{
		// hight light mouse clicked
		QRgb cl = m_viewer->fboImage().pixel(ev->pos());
		ldp::Float4 color(qRed(cl), qGreen(cl), qBlue(cl), qAlpha(cl));
		color /= 255.f;
		int id = svg::SvgAbstractObject::index_from_color(color);
		m_viewer->getSvgManager()->highlightShapeByIndex(m_lastHighlightShapeId, id);
		m_lastHighlightShapeId = id;
	}

	if (m_viewer->buttons() == Qt::MidButton)
	{
		float l = m_viewer->camera().getFrustumLeft();
		float r = m_viewer->camera().getFrustumRight();
		float t = m_viewer->camera().getFrustumTop();
		float b = m_viewer->camera().getFrustumBottom();
		float dx = (r - l) / float(m_viewer->width()) * (ev->pos().x() - m_viewer->lastMousePos().x());
		float dy = (b - t) / float(m_viewer->height()) * (ev->pos().y() - m_viewer->lastMousePos().y());
		l -= dx;
		r -= dx;
		t -= dy;
		b -= dy;
		m_viewer->camera().setFrustum(l, r, t, b, 
			m_viewer->camera().getFrustumNear(), 
			m_viewer->camera().getFrustumFar());
	}
}

void AbstractEventHandle::wheelEvent(QWheelEvent *ev)
{
	float s = 1.2f;
	if (ev->delta() < 0)
		s = 1.f / s;

	float l = m_viewer->camera().getFrustumLeft();
	float r = m_viewer->camera().getFrustumRight();
	float t = m_viewer->camera().getFrustumTop();
	float b = m_viewer->camera().getFrustumBottom();
	float dx = float(ev->pos().x()) / float(m_viewer->width()) * (r - l);
	float dy = float(ev->pos().y()) / float(m_viewer->height()) * (b - t);

	r = dx + l + (r - l - dx) * s;
	l = dx + l - dx * s;
	b = dy + t + (b - t - dy) * s;
	t = dy + t - dy * s;
	m_viewer->camera().setFrustum(l, r, t, b, 
		m_viewer->camera().getFrustumNear(), m_viewer->camera()
		.getFrustumFar());
}

void AbstractEventHandle::keyPressEvent(QKeyEvent *ev)
{

}

void AbstractEventHandle::keyReleaseEvent(QKeyEvent *ev)
{

}
#include<qtpch.h>
#include"OpenGLFramebuffer.h"
#include<glad/glad.h>
namespace Quantum {
	static const uint32_t s_MaxFramebufferSize = 8192;
	namespace Utils {
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}
		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			GLValidate(glCreateTextures(TextureTarget(multisampled), count, outID));
		}
		static void BindTexture(bool multisampled, uint32_t id)
		{
			GLValidate(glBindTexture(TextureTarget(multisampled), id));
		}
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				GLValidate(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE));
			}
			else
			{
				GLValidate(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,format, GL_UNSIGNED_BYTE, nullptr));

				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			}
			GLValidate(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0));
		}
		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				GLValidate(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
			}
			else
			{
				GLValidate(glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height));

				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
				GLValidate(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			}
			GLValidate(glFramebufferTexture2D(GL_FRAMEBUFFER,attachmentType, TextureTarget(multisampled), id, 0));
		}
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}

			return false;
		}
		static GLenum	QuantumFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			QT_CORE_ASSERT(false);
			return 0;
		}
	 }
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		:m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))	
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}
		Invalidate();
	}
	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		GLValidate(glDeleteFramebuffers(1, &m_RendererID));
		GLValidate(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
		GLValidate(glDeleteTextures(1, &m_DepthAttachment));
	}
	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			GLValidate(glDeleteFramebuffers(1, &m_RendererID));
			GLValidate(glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data()));
			GLValidate(glDeleteTextures(1, &m_DepthAttachment));

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}
		GLValidate(glCreateFramebuffers(1, &m_RendererID));
		GLValidate(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));

		bool multisample = m_Specification.Samples > 1;

		//Attachments
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());
			for (size_t i = 0;i< m_ColorAttachments.size();i++)
			{
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8,GL_RGBA, m_Specification.Width, m_Specification.Height, (int)i);
					break;
			case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I,GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}
		if (m_ColorAttachments.size() > 1)
		{
			QT_CORE_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
			GLValidate(glDrawBuffers(m_ColorAttachments.size(), buffers));
		}
		else if (m_ColorAttachments.empty())
		{
			//Only depth-pass
			GLValidate(glDrawBuffer(GL_NONE));
		}
		QT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		GLValidate(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	}
	void OpenGLFramebuffer::Bind()
	{
		GLValidate(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		GLValidate(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
	}
	void OpenGLFramebuffer::UnBind()
	{

		GLValidate(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			QT_CORE_WARN("Attempted to Resize Framebuffer to {0},{1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();
	}
	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		QT_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		GLValidate(glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex));
		int pixelData;
		GLValidate(glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData));
		return pixelData;
	}
	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		QT_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		GLValidate(glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::QuantumFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value));
	}
}
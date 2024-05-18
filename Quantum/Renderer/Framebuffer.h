#pragma once
#include<Quantum/Core/Base.h>
namespace Quantum {
	enum class FramebufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,
		//Depth/Stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	};
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};
	struct FramebufferSpecification {
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		//framebuffer format
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};
	class Framebuffer {
	public:
		virtual ~Framebuffer() = default;
		virtual void Bind() = 0 ;
		virtual void UnBind() =  0;

		virtual void Resize(uint32_t width, uint32_t heihgt) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0)const = 0;
		virtual const  FramebufferSpecification& GetSpecification() const = 0;
		static Ref<	Framebuffer>Create(const FramebufferSpecification& spec);


	};

}

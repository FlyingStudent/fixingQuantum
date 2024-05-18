#pragma once
#include"Quantum/Renderer/Texture.h"
#include<filesystem>
namespace Quantum {
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

	};

}

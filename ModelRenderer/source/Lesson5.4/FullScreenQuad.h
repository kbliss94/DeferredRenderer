#pragma once

#include <wrl.h>
#include <d3d11_2.h>
#include <functional>
#include <vector>
#include "DrawableGameComponent.h"
#include "Game.h"

namespace Rendering
{
	class FullScreenQuad final : public Library::DrawableGameComponent
	{
		RTTI_DECLARATIONS(FullScreenQuad, DrawableGameComponent)

	public:
		FullScreenQuad(Library::Game& game);
		FullScreenQuad(const FullScreenQuad& rhs) = delete;
		FullScreenQuad& operator=(const FullScreenQuad& rhs) = delete;

		void SetCustomUpdateMaterial(std::function<void()> callback);

		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		UINT mIndexCount;
		std::function<void()> mCustomUpdateMaterial;
	};
}
///////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

// The TextureClass encapsulates the loading, unloading and accessing of a single texture resource. 
// For each texture needed an object of this class must be instantiated.

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	// The first two functions will load a texture from a given file name and unload that texture when it is no longer needed.
	bool Initialize(ID3D11Device*, const char*);
	void Shutdown();

	// The GetTexture function returns a pointer to the texture resource so that it can be used for rendering by shaders.
	ID3D11ShaderResourceView* GetTexture();

private:
	// Private texture resource.
	ID3D11ShaderResourceView* m_texture;
};

#endif
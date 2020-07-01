
-- Window config
Width = 1920
Height = 1080
Title = "Hello World"
Version = 0.1

-- Shaders used by the sandbox
    -- 1 = shader type
    -- 2 = shader path
    -- 3 = shader entry point

Shaders = {}
Shaders[0] = { "fragment", "Mage/Resources/Shaders/fragment.sprv", "main" }
Shaders[1] = { "vertex", "Mage/Resources/Shaders/vertex.sprv", "main" }

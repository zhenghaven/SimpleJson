file(GLOB headers_internal "Internal/*.hpp")

install(FILES ${headers_internal}
	DESTINATION include/SimpleJson/Internal
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

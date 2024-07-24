typedef struct Sprite
{
	Gfx_Image* image;
	Vector2 size;
	Vector2 clickableSize;
	Vector2 origin;
} Sprite;

typedef enum SpriteID
{
	s_nil = 0,
	s_player,
	s_obj0,
	s_door0,
	s_tree0,
	s_rock0,
	s_flower,
	s_inv_flower0,
	s_inv_flower1,
	s_inv_flower2,
	s_inv_item0,
	s_inv_item1,
	s_MAX,
} SpriteID;
Sprite sprites[s_MAX];

typedef struct Item
{
	// ..
} Item;

typedef enum ItemID
{
	i_nil = 0,
	i_key,
	i_letter,
	i_wrench,
	i_rock,
	i_flower_pink,
	i_flower_blue,
	i_flower_yellow,
	i_MAX,
} ItemID;
Item items[i_MAX];

typedef struct Room
{
	// .. map of sprites w pos, background, walkbox, etc
} Room;

typedef enum RoomID
{
	r_nil = 0,
	r_foyer,
	r_library,
	//..
	r_MAX,
} RoomID;
Room rooms[r_MAX];

typedef enum VerbState
{
	v_nil = 0,
	v_look = 1,
	v_use = 2,  // --> use, open, pickup, etc interact
	v_talk = 3,
	// etc...
	v_MAX = 4,
} VerbState;

typedef enum EntityFlags
{
    ACTIVE = 1 << 0,
    //etc

} EntiityFlags;

typedef enum EntityType
{
	nil = 0,
	t_player = 1,
	t_npc = 2,
	t_room = 3,
	t_object = 4,
	t_item = 5,
	t_door = 6,
	t_tree = 7,
	t_rock = 8,
	t_flower = 9,
} EntityType;

typedef struct Entity // MegaStruct approach
{
	bool isValid;
	EntityType type;
    u64 flags;
	Vector2 pos;
	SpriteID spriteID;
	ItemID itemID;
	RoomID roomID;
	bool clickable;
	string hoverText;
	VerbState verbState;
} Entity;

Sprite* getSprite(SpriteID spriteID)
{
	if(spriteID >= 0 && spriteID < s_MAX) return &sprites[spriteID];
	else return &sprites[0];
}

Item* getItem(ItemID itemID)
{
	if (itemID >= 0 && itemID < i_MAX) return &items[itemID];
	else return &items[0];
}

#define MAX_ENTITY_COUNT 1024

typedef struct World
{
	Entity entities[MAX_ENTITY_COUNT];
} World;

World* world = 0;

typedef struct WorldFrame
{
	Entity* activeEntity;
} WorldFrame;

WorldFrame worldFrame;

Entity* createEntity(EntityType type, SpriteID spriteID, ItemID itemID, Vector2 pos, string hoverText, bool clickable, u64 flags) // flags - clickable, active, render etc..
{
	Entity* entityFound = 0;
	for (int i = 0; i < MAX_ENTITY_COUNT; i++)
	{
		Entity* e = &world->entities[i];
		if (!e->isValid) // ie if no entity exists here, memory is free, use that
		{
			entityFound = e;
			break;
		}
	}
	assert(entityFound, "No more free entities");
	entityFound->isValid = true;
	entityFound->type = type;
	entityFound->flags = flags;
	entityFound->pos = pos;
	entityFound->spriteID = spriteID;
	entityFound->itemID = itemID;
	entityFound->clickable = clickable;
	if (hoverText.count != 0) entityFound->hoverText = hoverText;
	else entityFound->hoverText = STR("");
	if (entityFound->type == t_player)  entityFound->verbState = v_look;
	else entityFound->verbState = v_nil;

	return entityFound;
}

void destroyEntity(Entity* e)
{
	memset(e, 0, sizeof(Entity));
}

void loadRoom(RoomID roomID)
{
	// ..
}

void loadSprite(SpriteID spriteID, string path, Vector2 clickableSize, Vector2 origin) // no default values in c -> reason to use c++
{
		Sprite sprite;
		Gfx_Image* image = load_image_from_disk(path, get_heap_allocator());
		// assert(image, "failed to load image")
		sprite.image = image;
		sprite.size = v2(sprite.image->width, sprite.image->height);

		if (clickableSize.x != 0.0)  sprite.clickableSize.x = clickableSize.x;
		else sprite.clickableSize.x = sprite.size.x;
		if (clickableSize.y != 0.0)  sprite.clickableSize.y = clickableSize.y;
		else sprite.clickableSize.y = sprite.size.y;

		if (origin.x) sprite.origin = origin;									// bottom center origin point of the hotspot relative to size
		else sprite.origin = v2(sprite.size.x * 0.5, 0.0);
		
		sprites[spriteID] = sprite;

		// assert?
}
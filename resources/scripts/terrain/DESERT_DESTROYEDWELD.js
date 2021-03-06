var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_DESTROYEDWELD.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Destroyed Weld");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else
        {
            terrain.loadBaseTerrain("DESERT");
        }
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("PIPELINE,SNOW_PIPELINE,DESERT_PIPELINE", false, false, GameEnums.Directions_Direct, true);
        var x = terrain.getX();
        var y = terrain.getY();
        if (typeof map !== 'undefined')
        {
            if (map.onMap(x, y + 1))
            {
                var building = map.getTerrain(x, y + 1).getBuilding();
                if (building !== null &&
                        building.getBuildingID() === "ZBLACKHOLE_FACTORY" &&
                        building.getX() - 1 === x && building.getY() - 4 === y)
                {
                    if (surroundings.indexOf("+W") >= 0)
                    {
                        surroundings = surroundings.replace("+W", "+S+W");
                    }
                    else
                    {
                        surroundings += "+S";
                    }
                }
            }
        }
        if ((surroundings === ""))
        {
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
        }
        else if ((surroundings === "+N+S"))
        {

            terrain.loadBaseSprite("desert_destroyedweld+N+S");
        }
        else if ((surroundings === "+E+W"))
        {
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
        }
        else
        {
            terrain.loadBaseSprite("desert_destroyedweld+E+W");
        }
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        var surroundings = terrain.getSurroundings("PIPELINE,DESERT_PIPELINE,SNOW_PIPELINE", false, false, GameEnums.Directions_Direct, true);
        if ((surroundings === "+E+W") || (surroundings === "+N+S"))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };

    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain, but it reduces the firerange of indirect units by 1.");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["desert_destroyedweld+E+W",
                "desert_destroyedweld+N+S"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_desert";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_desert";
    };
};
Constructor.prototype = TERRAIN;
var DESERT_DESTROYEDWELD = new Constructor();

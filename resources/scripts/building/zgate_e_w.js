var Constructor = function()
{
    this.init = function (building)
    {
        building.setHp(100);
        building.setAlwaysVisble(true);
    };
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        building.loadSprite("gate+E+W", false);
        building.loadSpriteV2("gate+E+W+mask", GameEnums.Recoloring_Table);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Gate");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.onDestroyed = function(building)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        map.replaceTerrain("PLAINS_DESTROYED", x, y);
        map.getTerrain(x, y).loadSprites();
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+land.wav");
    };

    this.getDescription = function()
    {
        return qsTr("A gate through a wall can be crossed by all allied players. Can be destroyed by other players");
    };
}

Constructor.prototype = BUILDING;
var ZGATE_E_W = new Constructor();

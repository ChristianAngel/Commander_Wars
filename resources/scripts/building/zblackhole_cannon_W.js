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
        building.loadSprite("black_cannon+W", false);
        building.loadSpriteV2("black_cannon+W+mask", GameEnums.Recoloring_Table);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_CANNON_FIRE";
    };
    this.startOfTurn = function(building)
    {
        building.setFireCount(1);
    };
    this.getName = function()
    {
        return qsTr("Black Cannon");
    };
    this.getActionTargetFields = function(building)
    {
        return globals.getShotFields(1, 10, -1, 0);
    };
    this.getActionTargetOffset = function(building)
    {
        // offset for large buildings since there reference point is bound to the lower right corner.
        return Qt.point(-2, -1);
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 3;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 3;
    };
    this.canBuildingBePlaced = function(terrain, building)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZBLACKHOLE_CANNON_W.getBuildingWidth(), ZBLACKHOLE_CANNON_W.getBuildingHeigth());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getIsAttackable = function(building, x, y)
    {
        var buildX = building.getX();
        var buildY = building.getY();
        if (y === buildY && buildX - 1 === x)
        {
            return true;
        }
        return false;
    };
    this.getDamage = function(building, unit)
    {
        return 5;
    };
    this.getBuildingTargets = function()
    {
        return GameEnums.BuildingTarget_Enemy;
    };
    this.onDestroyed = function(building)
    {
        // called when the terrain is destroyed and replacing of this terrain starts
        var x = building.getX();
        var y = building.getY();
        var animation2 = GameAnimationFactory.createAnimation(0, 0);
        animation2.addSprite2("white_pixel", 0, 0, 3200, map.getMapWidth(), map.getMapHeight());
        animation2.addTweenColor(0, "#00FFFFFF", "#FFFFFFFF", 3000, true);
        audio.playSound("explosion+land.wav");
        map.getTerrain(x, y).loadBuilding("ZBLACK_BUILDING_DESTROYED");
    };
    this.getShotAnimation = function(building)
    {
        var animation = GameAnimationFactory.createAnimation(building.getX(), building.getY(), 70);
        animation.addSprite("blackhole_shot_west", -map.getImageSize() * 3.0, -map.getImageSize() * 3.0, 0, 1.5);
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("Black Hole Cannon that can deal 5 damage to a single unit in a large range.");
    };
}

Constructor.prototype = BUILDING;
var ZBLACKHOLE_CANNON_W = new Constructor();

var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        // neutral player
        building.loadSprite("silo", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_silo";
    };
}

Constructor.prototype = BUILDING;
var SILO = new Constructor();

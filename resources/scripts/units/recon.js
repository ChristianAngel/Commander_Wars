var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon2ID("WEAPON_RECON_MG");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(80);
        unit.setMaxFuel(80);
        unit.setBaseMovementPoints(8);
        unit.setCosts(4000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(5);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("recon", false);
        unit.loadSprite("recon+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_A";
    };
}

Constructor.prototype = UNIT;
var RECON = new Constructor();
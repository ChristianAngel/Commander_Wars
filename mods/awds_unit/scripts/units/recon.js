var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(-1);
        unit.setMaxAmmo1(-1);
        unit.setWeapon1ID("WEAPON_RECON_MG");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(80);
        unit.setMaxFuel(80);
        unit.setBaseMovementPoints(8);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(5);
    };
    this.getBaseCost = function()
    {
        return 4000;
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
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.25);
        animation.loadSprite(unitID + "+walk", false, 1.25);
        animation.setSound("movetire.wav", -2);
        return animation;
    };
    this.getName = function()
    {
        return qsTr("Recon");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var RECON = new Constructor();

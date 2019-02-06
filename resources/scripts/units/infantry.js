var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_INFANTRY_MG");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
        unit.setCosts(1500);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        // bh and bg have the same sprites
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        // load sprites
        unit.loadSprite("infantry+" + armyName, false);
        unit.loadSprite("infantry+" + armyName +"+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_CAPTURE,ACTION_WAIT,ACTION_JOIN";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
		var zoom = 1;
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
		else
		{
			zoom = 1.5;
		}
        animation.loadSprite("infantry+" + armyName + "+walk+mask", true, zoom);
        animation.loadSprite("infantry+" + armyName + "+walk", false, zoom);
        animation.setSound("resources/sounds/movefeet.wav", -1);
        return animation;
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
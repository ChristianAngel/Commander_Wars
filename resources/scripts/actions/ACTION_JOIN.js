var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
		var targetUnit = action.getMovementTarget();
        if ((unit.getHasMoved() === true))
        {
            return false;
        }
        if (((actionTargetField.x !== targetField.x) || (actionTargetField.y !== targetField.y)) &&
            (targetUnit !== null))
        {
            if ((targetUnit.getOwner() === unit.getOwner()) &&
                (targetUnit.getUnitID() === unit.getUnitID()) &&
				(targetUnit.getHpRounded() < 10))
			{
				return true;
			}
        }
        return false;
        
    };
    this.getActionText = function()
    {
        return qsTr("Join");
    };
    this.getIcon = function()
    {
        return "join";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.postAnimationTargetUnit = null;
    this.postAnimationUnit = null;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        ACTION_JOIN.postAnimationUnit = action.getTargetUnit();
        ACTION_JOIN.postAnimationTargetUnit = action.getMovementTarget();
        var animation = this[ACTION_JOIN.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_JOIN", "performPostAnimation");
    };
    this.performPostAnimation = function()
    {
        // todo join units
        var ammo1 = ACTION_JOIN.postAnimationUnit.getAmmo1() + ACTION_JOIN.postAnimationTargetUnit.getAmmo1();
        var maxValue = ACTION_JOIN.postAnimationUnit.getMaxAmmo1();
        if (ammo1 > maxValue)
        {
            ammo1 = maxValue;
        }
        maxValue = ACTION_JOIN.postAnimationUnit.getMaxAmmo2();
        var ammo2 = ACTION_JOIN.postAnimationUnit.getAmmo2() + ACTION_JOIN.postAnimationTargetUnit.getAmmo2();
        if (ammo2 > maxValue)
        {
            ammo2 = maxValue;
        }
        maxValue = ACTION_JOIN.postAnimationUnit.getMaxFuel();
        var fuel = ACTION_JOIN.postAnimationUnit.getFuel() + ACTION_JOIN.postAnimationTargetUnit.getFuel();
        if (fuel > maxValue)
        {
            fuel = maxValue;
        }
        var hp = ACTION_JOIN.postAnimationUnit.getHpRounded() + ACTION_JOIN.postAnimationTargetUnit.getHpRounded();
        if (hp > 10)
        {
            var overHeal = hp - 10;
            // todo earn money :)
            var income = ACTION_JOIN.postAnimationTargetUnit.getCosts() * (overHeal / 10);
            ACTION_JOIN.postAnimationTargetUnit.getOwner().addFonds(income);
            hp = 10;
        }

        ACTION_JOIN.postAnimationTargetUnit.setAmmo1(ammo1);
        ACTION_JOIN.postAnimationTargetUnit.setAmmo2(ammo2);
        ACTION_JOIN.postAnimationTargetUnit.setFuel(fuel);
        ACTION_JOIN.postAnimationTargetUnit.setHp(hp);
        ACTION_JOIN.postAnimationUnit.removeUnit();
        // disable unit commandments for this turn
        ACTION_JOIN.postAnimationTargetUnit.setHasMoved(true);
        ACTION_JOIN.postAnimationTargetUnit = null;
        ACTION_JOIN.postAnimationUnit = null;
    };


}

Constructor.prototype = ACTION;
var ACTION_JOIN = new Constructor();
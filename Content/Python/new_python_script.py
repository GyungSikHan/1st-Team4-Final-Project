import unreal

editor_actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = editor_actor_subsystem.get_all_level_actors()

for actor in actors:
    actor_class_name = actor.get_class().get_name()
    
    if actor_class_name.startswith("LandscapeStreamingProxy") or actor_class_name.startswith("LandscapeProxy"):
        try:
            if actor.has_editor_property("HLODLayer"):
                actor.set_editor_property("HLODLayer", None)
                unreal.log(f"HLODLayer cleared for {actor.get_name()}")
            else:
                unreal.log_warning(f"{actor.get_name()} has no HLODLayer property.")
        except Exception as e:
            unreal.log_warning(f"Failed to reset HLODLayer on {actor.get_name()}: {e}")

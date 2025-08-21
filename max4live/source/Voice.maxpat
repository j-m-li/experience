{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 9,
			"minor" : 0,
			"revision" : 7,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 34.0, 87.0, 641.0, 360.0 ],
		"gridsize" : [ 15.0, 15.0 ],
		"boxes" : [ 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-9",
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 97.0, 39.0, 69.0, 39.0 ],
					"suppressinlet" : 1,
					"text" : "Grain phase",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-45",
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 27.0, 39.0, 52.0, 39.0 ],
					"suppressinlet" : 1,
					"text" : "Position",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-40",
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 401.0, 39.0, 76.0, 39.0 ],
					"suppressinlet" : 1,
					"text" : "Cloud voices",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 0,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-46",
					"linecount" : 2,
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 221.0, 212.0, 61.0, 52.0 ],
					"suppressinlet" : 1,
					"text" : "Audio Output R",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 0,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-43",
					"linecount" : 2,
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 27.0, 212.0, 61.0, 52.0 ],
					"suppressinlet" : 1,
					"text" : "Audio Output L",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-8",
					"linecount" : 2,
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 329.0, 26.0, 63.0, 52.0 ],
					"suppressinlet" : 1,
					"text" : "Grain smoothing",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-41",
					"linecount" : 2,
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 247.0, 26.0, 72.0, 52.0 ],
					"suppressinlet" : 1,
					"text" : "Position randomness",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-42",
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 176.0, 39.0, 59.0, 39.0 ],
					"suppressinlet" : 1,
					"text" : "Grain size",
					"textjustification" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 0,
					"bubbleusescolors" : 1,
					"fontsize" : 11.0,
					"id" : "obj-7",
					"linecount" : 3,
					"maxclass" : "live.comment",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 444.0, 258.0, 148.0, 65.0 ],
					"suppressinlet" : 1,
					"text" : "Notice how the volume envelopes of the two voices add up to a constant value.",
					"textjustification" : 0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 425.0, 80.0, 28.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(float)", "cloud", "voices" ]
					}
,
					"text" : "in 6"
				}

			}
, 			{
				"box" : 				{
					"grid" : 2,
					"id" : "obj-1",
					"maxclass" : "live.scope~",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 425.0, 188.0, 184.0, 68.0 ],
					"vertical_divisions" : 2
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 523.0, 80.0, 45.0, 22.0 ],
					"text" : "open 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 472.0, 80.0, 45.0, 22.0 ],
					"text" : "open 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-262",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 36.0, 188.0, 42.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(signal)", "left", "audio", "out" ]
					}
,
					"text" : "out~ 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-263",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 230.0, 188.0, 42.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(signal)", "right", "audio", "out" ]
					}
,
					"text" : "out~ 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 269.0, 80.0, 28.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(float", ",", "ms)", "position", "randomness" ]
					}
,
					"text" : "in 4"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-256",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 114.0, 80.0, 35.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(signal)", "grain", "phase" ]
					}
,
					"text" : "in~ 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-258",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 347.0, 80.0, 28.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(float)", "grain", "smoothing" ]
					}
,
					"text" : "in 5"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-260",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 36.0, 80.0, 35.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(signal)", "position" ]
					}
,
					"text" : "in~ 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-261",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 191.0, 80.0, 28.0, 22.0 ],
					"saved_object_attributes" : 					{
						"attr_comment" : [ "(float", ",", "ms)", "grain", "size" ]
					}
,
					"text" : "in 3"
				}

			}
, 			{
				"box" : 				{
					"color" : [ 0.0, 1.0, 0.694117647058824, 1.0 ],
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 6,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "signal", "signal" ],
					"patching_rect" : [ 36.0, 149.0, 408.0, 22.0 ],
					"text" : "poly~ HalfVoice 2 @target 0"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-11", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-262", 0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-263", 0 ],
					"source" : [ "obj-11", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 1 ],
					"source" : [ "obj-256", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 4 ],
					"source" : [ "obj-258", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-260", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 2 ],
					"source" : [ "obj-261", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 3 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 5 ],
					"source" : [ "obj-5", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "HalfVoice.maxpat",
				"bootpath" : "D:/src/max/Max for Live Devices/Grain Cloud Simpler Project",
				"patcherrelativepath" : "../../max/Max for Live Devices/Grain Cloud Simpler Project",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "Smoothstep~.maxpat",
				"bootpath" : "D:/src/max/Max for Live Devices/Grain Cloud Simpler Project",
				"patcherrelativepath" : "../../max/Max for Live Devices/Grain Cloud Simpler Project",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0,
		"oscreceiveudpport" : 0
	}

}

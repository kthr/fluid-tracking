(* ::Package:: *)

If[NameQ["LabelingUnload"],
	LabelingUnload[]
];


lib::nolink="Can't find \"libGraphCut\" at `1`. Some functions may work.";
library=FileNameJoin[{"/Users/kthierbach/Documents/projects/eclipse/Labeling/build/libLabeling"}];
$libGraphCut=FindLibrary[library];
$librariesFound=If[$libGraphCut === $Failed,
    Message[lib::nolink, library];
    False,
    llLabeling=LibraryFunctionLoad[$libGraphCut,"llLabeling",{{Integer,2,"Constant"},{Integer,2,"Constant"},Integer,Integer,Real,Real,Real,Real},{Integer,2}];
    True
];


Clear[Labeling]
Labeling::ilab="Label 1 is reserved for appearing objects don't use it in your label matrix!!";
Options[Labeling] = {Lambda->1, Mu->1, C0->0., C1->1.,BitDepth->"Byte"};
 Labeling[label:{{__Integer}..}, image_Image,OptionsPattern[]]:=Block[
    {parameters,beta,bitDepth,data,newLabel},
    parameters = OptionValue[{C0, C1, Lambda, Mu}];
    labels=Union@Flatten@label;
    If[MemberQ[labels,1],
       Message[Labeling::ilab];
       Abort[]
       ];
    numLabels=Length@labels+1;
	bitDepth = OptionValue[BitDepth];
	If[ImageChannels[image]!=1,
		Print["Image is not a grayscale image, converting..."];
		data = ImageData[ColorConvert[image,"Grayscale"],bitDepth],
		data = ImageData[image,bitDepth]
	];
    newLabel=llLabeling[label,data,Switch[bitDepth,"Byte",8,"Bit16",16],numLabels,Sequence@@parameters];
    newLabel
]


Clear[LabelingUnload]
LabelingUnload[]:=(
	LibraryFunctionUnload[llLabeling]
)

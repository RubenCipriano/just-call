import { ChannelType } from "../enum/ChannelType.enum";
import { Message } from "./Message";
import { Person } from "./Person";

export class Channel {
    public persons!: Person[]

    public messages!: Message[]

    public type!: ChannelType

    public name!: string

    public readonly: boolean = true;
}